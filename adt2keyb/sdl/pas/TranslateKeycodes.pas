// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure TranslateKeycodes;

var
  i,j: Integer;
  modkeys: SDLMod;

begin
  // translate SDL_Keycodes to scancodes
  For i := 0 to SDLK_LAST do
    For j := 0 to PRED(SYMTABSIZE) do
      If (i = symtab[j*10+9]) then
        keydown[symtab[j*10]] := keystate^[i];

  // read capslock and numlock state
  modkeys := SDL_GetModState;
  _capslock := (modkeys AND KMOD_CAPS) <> 0;
  _numlock := (modkeys AND KMOD_NUM) <> 0;
end;
