// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

bool keypressed () {
  realtime_gfx_poll_proc ();

  draw_screen ();

  // Filter out CTRL+TAB combo as it is handled within timer routine
  if (ctrl_tab_pressed ()) {
    keyboard_reset_buffer ();

    return false;
  } else
    return kbhit ();
}
