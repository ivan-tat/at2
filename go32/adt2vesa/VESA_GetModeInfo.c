// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

int VESA_GetModeInfo (uint16_t Mode, uint32_t DOSAddr) {
  __dpmi_regs regs;

  regs.d.eax = 0x4F01; // AX = Return mode information
  regs.d.ecx = Mode; // CX = Mode number
  regs.d.edi = DOSAddr & 0xF;
  regs.x.ds = DOSAddr >> 4;
  regs.x.es = DOSAddr >> 4; // ES:DI = Pointer to ModeInfoBlock structure

  if (__dpmi_simulate_real_mode_interrupt (0x10, &regs)) {
    VESA_CallStatus = VESACS_DE;
    VESA_Error = 1; // Function call failed
    return -1;
  } else
    return _VESA_CheckStatus (regs.x.ax);
}
