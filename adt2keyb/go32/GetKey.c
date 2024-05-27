// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

ExtKeyCode GetKey (void) {
  int x;

  no_status_refresh = false;

  while (!keypressed ()) {
    realtime_gfx_poll_proc ();

    draw_screen ();

    if (seconds_counter >= ssaver_time)
      screen_saver ();
  }

  x = getkey ();
  return (x & 0x100) ? MK_EKEYEA (x & 0xFF)
                     : MK_EKEYSC (inportb (0x60) & 0x7F, x);
}
