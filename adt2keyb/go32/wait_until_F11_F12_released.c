// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void wait_until_F11_F12_released (void) {
  DBG_ENTER ("wait_until_F11_F12_released");

  do {
    realtime_gfx_poll_proc ();
    draw_screen ();
    keyboard_reset_buffer ();
    if (inportb (0x60) & 0x80)
      memset (keydown, 0, sizeof (keydown));
  } while (keydown[SC_F11] || keydown[SC_F12]);

  DBG_LEAVE (); //wait_until_F11_F12_released
}
