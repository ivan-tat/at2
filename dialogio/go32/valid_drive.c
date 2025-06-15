// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static bool valid_drive (char drive)
{
  bool r = false;
  int32_t sel, seg;
  uint8_t data[40]; // HINT: (FPC) start index 0
  __dpmi_regs regs;

  DBG_ENTER ("valid_drive");

  seg = __dpmi_allocate_dos_memory ((sizeof (data) + 15) / 16, &sel);
  if (seg < 0)
    goto _error; // ERROR: failed to allocate DOS memory

  data[0] = UpCase (drive);
  data[1] = ':';
  data[2] = '\x00';
  dosmemput (data, sizeof (data), seg * 16);

  regs.x.ax = 0x2906; // Fn 29h - Parse filename, AL = use FCB drive byte and filename as a default
  regs.x.ds = seg; // DS:SI = source text line to parse
  regs.x.si = 0;
  regs.x.es = seg; // ES:DI = buffer to hold resulting unopened FCB
  regs.x.di = 3;
  if (!__dpmi_simulate_real_mode_interrupt (0x21, &regs))
    // AL = FFh if invalid drive was specified
    // DS:SI updated: points to the character just past the filename
    // ES:DI preserved: points to the unopened FCB
    r = (regs.h.al != 0xFF) && !phantom_drive (drive);

_error:
  if (seg >= 0)
    __dpmi_free_dos_memory (sel);

  DBG_LEAVE (); //EXIT //valid_drive
  return r;
}
