// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void VESA_SwitchBank (uint8_t bank) {
  __dpmi_regs regs;
  uint8_t granularity;

  regs.d.res = 0;
  regs.x.sp = 0;
  regs.x.ss = 0;
  regs.d.eax = 0x4F05; // VBE Display Window Control
  regs.d.ebx = 0; // Set Memory Window A
  switch (ModeInfoBlock.WinGranularity) {
  case 32:
    granularity = 5;
    break;
  case 16:
    granularity = 4;
    break;
  case 8:
    granularity = 3;
    break;
  case 4:
    granularity = 2;
    break;
  case 2:
    granularity = 1;
    break;
  case 1:
  default:
    granularity = 0;
    break;
  }
  regs.d.edx = bank << granularity; // Window number in video memory in window
                                    // granularity units (Set Memory Window only)
  if (__dpmi_simulate_real_mode_interrupt (0x10, &regs)) {
    // FIXME: no check for error in original code
  }
}
