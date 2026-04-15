// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#if ADT2PLAY

bool keypressed (void) {
  __dpmi_regs regs;

  regs.d.res = 0;
  regs.x.sp = 0;
  regs.x.ss = 0;
  regs.h.ah = 1;  // Query keyboard status/preview key
  __dpmi_simulate_real_mode_interrupt (0x16, &regs);
  // Output:
  //   zf = 1: no keys in buffer
  //   zf = 0: key is pressed
  //     al = ASCII character code or extended ASCII keystroke
  //     ah = scan code

  return regs.x.flags & (1 << 6) ? false : true;
}

#else // !ADT2PLAY

bool keypressed (void) {
  realtime_gfx_poll_proc ();

  draw_screen ();

  // Filter out CTRL+TAB combo as it is handled within timer routine
  if (ctrl_tab_pressed ()) {
    keyboard_reset_buffer ();

    return false;
  } else
    return kbhit ();
}

#endif // !ADT2PLAY
