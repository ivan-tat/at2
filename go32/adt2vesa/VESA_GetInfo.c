// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Returns 0 on success, error code otherwise.
static int VESA_GetInfo (tVESA_Info *p)
{
  int status = 0;
  int32_t sel, seg;
  __dpmi_regs regs;

  seg = __dpmi_allocate_dos_memory ((sizeof (tVESA_Info) + 15) / 16, &sel);
  if (seg < 0)
  {
    status = 1; // ERROR: failed to allocate `tVESA_Info' structure
    goto _error;
  }

  p->Signature = MK_UINT32 ('V', 'B', 'E', '2');
  memset ((uint8_t *) p + 4, 0, sizeof (tVESA_Info) - 4);
  // Indicate VBE 2.0 information is desired and the information
  // block is 512 bytes in size
  dosmemput (p, 4, seg * 16);

  regs.d.eax = 0x4F00; // Return VBE Controller Information
  regs.x.ds = seg;
  regs.x.es = seg;     // ES:DI = Pointer to buffer in which to place
  regs.d.edi = 0;      // VbeInfoBlock structure
  if (__dpmi_simulate_real_mode_interrupt (0x10, &regs))
  {
    status = 2; // ERROR: failed to read `tVESA_Info' structure
    goto _error;
  }

  dosmemget (seg * 16, sizeof (tVESA_Info), p);

_error:
  if (seg >= 0)
    __dpmi_free_dos_memory (sel);

  return status;
}
