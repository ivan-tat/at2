// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

bool keypressed () {
  SDL_Event e;

  while (true) {
    keyboard_poll_input ();

    if (SDL_PeepEvents (&e, 1, SDL_PEEKEVENT, SDL_QUITMASK) > 0) {
      _force_program_quit = true;
      return true;
    }

    if (SDL_PeepEvents (&e, 1, SDL_PEEKEVENT, SDL_MOUSEEVENTMASK) > 0) {
      // Skip mouse events
      SDL_PeepEvents (&e, 1, SDL_GETEVENT, SDL_MOUSEEVENTMASK);
      continue;
    }

    if (SDL_PeepEvents (&e, 1, SDL_PEEKEVENT, SDL_KEYDOWNMASK) > 0) {
      if (e.key.keysym.sym >= SDLK_NUMLOCK) {
        // Skip modifier key presses
        SDL_PeepEvents (&e, 1, SDL_GETEVENT, SDL_KEYDOWNMASK);
        continue;
      } else
        return true;
    }

    return false;
  }
}
