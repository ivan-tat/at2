// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static bool is_last = false;
static char last = '\0';

static int readkey (void) {
  __dpmi_regs regs;

  if (is_last) {
    is_last = false;
    return last;
  } else {
    regs.d.res = 0;
    regs.x.sp = 0;
    regs.x.ss = 0;
    regs.h.ah = 0x10; // Read extended keyboard input
    __dpmi_simulate_real_mode_interrupt (0x16, &regs);
    // Output: al = ASCII character code or extended ASCII keystroke
    //         ah = scan code

    if ((regs.h.al == 0xE0) && regs.h.ah) {
      regs.h.al = 0;

    if (!regs.h.al) {
      is_last = true;
      last = regs.h.ah;
    }

    return regs.h.al;
  }
}
