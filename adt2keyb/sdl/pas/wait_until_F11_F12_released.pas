// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure wait_until_F11_F12_released;
begin
  _debug_str_ := 'ADT2KEYB.PAS:wait_until_F11_F12_released';
  Repeat
    draw_screen;
    SDL_PumpEvents;
    TranslateKeycodes;
  until NOT keydown[SC_F11] and NOT keydown[SC_F12];
  keyboard_reset_buffer;
end;
