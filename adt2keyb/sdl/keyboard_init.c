// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void keyboard_init (void) {
  SDL_EnableKeyRepeat (sdl_typematic_delay, sdl_typematic_rate);
  keystate = (bool *) SDL_GetKeyState (&numkeys);
}
