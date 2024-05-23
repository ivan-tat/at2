// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void TranslateKeycodes (void) {
  SDLKey k;
  SDLMod m;

  // Translate SDL_Keycodes to scancodes
  for (k = 0; k <= SDLK_LAST; k++) {
    unsigned i;

    for (i = 0; i < SYMTABSIZE; i++)
      if (symtab[i].SDLK == k)
        keydown[symtab[i].scan] = keystate[k];
  }

  // Read `Caps Lock' and `Num Lock' state
  m = SDL_GetModState ();
  _capslock = (m & KMOD_CAPS) != 0;
  _numlock = (m & KMOD_NUM) != 0;
}
