// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static uint16_t GetKey_proc (void) {
  while (true) {
    SDL_Event e;

    draw_screen ();

    if (SDL_PollEvent (&e)) {
      if ((e.type == SDL_QUIT) || _force_program_quit) {
        _force_program_quit = true;
        return kESC;
      }

      // Skip all other event except key presses
      if (e.type != SDL_KEYDOWN)
        continue;
      else {
        unsigned i;

        // Skip all modifier keys
        if (e.key.keysym.sym >= SDLK_NUMLOCK)
          continue;

        // Roll through `symtab', form correct `GetKey' value
        for (i = 0; i <= SYMTABSIZE; i++)
          if (e.key.keysym.sym == symtab[i].SDLK) {
            AT2ExtKeyCode k;

            // First check with modifier keys, order: ALT, CTRL, SHIFT (as DOS does)

            // ALT
            if (keydown[SC_LALT] || keydown[SC_RALT]) {
              k = symtab[i].alt;

              if (k == ATEK_NULL) // HINT: Intersects with unknown so should be placed first
                return k;
              else if (ATEK_IsUnknown (k)) // Impossible combination
                continue;
              else if (ATEK_IsKey (k) || ATEK_IsModifier (k)) // Found
                return k;
              else
                return MK_EKEYSC (symtab[i].scan, ATEK_GetChar (k)); // Character
            }

            // CTRL
            if (keydown[SC_LCTRL] || keydown[SC_RCTRL]) {
              k = symtab[i].ctrl;

              if (k == ATEK_NULL) // HINT: Intersects with unknown so should be placed first
                return k;
              else if (ATEK_IsUnknown (k)) // Impossible combination
                continue;
              else if (ATEK_IsKey (k) || ATEK_IsModifier (k)) // Found
                return k;
              else
                return MK_EKEYSC (symtab[i].scan, ATEK_GetChar (k)); // Character
            }

            // SHIFT
            if (keydown[SC_LSHIFT] || keydown[SC_RSHIFT]) {
              k = symtab[i].shift;

              if (_capslock)
                k = symtab[i].caps_sh;
              else if (_numlock)
                k = symtab[i].num_sh;

              if (k == ATEK_NULL) // HINT: Intersects with unknown so should be placed first
                return k;
              else if (ATEK_IsUnknown (k)) // Impossible combination
                continue;
              else if (ATEK_IsKey (k) || ATEK_IsModifier (k)) // Found
                return k;
              else
                return MK_EKEYSC (symtab[i].scan, ATEK_GetChar (k)); // Character
            }

            // Normal symbol (ASCII)
            k = symtab[i].key;

            if (_capslock)
              k = symtab[i].caps;
            else if (_numlock)
              k = symtab[i].num;

            if (k == ATEK_NULL) // HINT: Intersects with unknown so should be placed first
              return k;
            else if (ATEK_IsUnknown (k)) // Impossible combination
              continue;
            else if (ATEK_IsKey (k) || ATEK_IsModifier (k)) // Found
              return k;
            else
              return MK_EKEYSC (symtab[i].scan, ATEK_GetChar (k)); // Character
          }
      }
    }
  }
}

uint16_t GetKey (void) {
  do {
    draw_screen ();
  } while (!keypressed ());

  // Filter out CTRL+TAB combo as it is handled within timer routine
  if (ctrl_tab_pressed ()) {
    draw_screen ();
    keyboard_reset_buffer ();
    return ATEK_UNKNOWN;
  } else
    return GetKey_proc ();
}
