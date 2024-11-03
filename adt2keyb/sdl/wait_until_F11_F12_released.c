// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void wait_until_F11_F12_released (void) {
  DBG_ENTER ("wait_until_F11_F12_released");

  do {
    draw_screen ();
    SDL_PumpEvents ();
    TranslateKeycodes ();
  } while (keydown[SC_F11] || keydown[SC_F12]);

  keyboard_reset_buffer ();

  DBG_LEAVE (); //wait_until_F11_F12_released
}
