// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure wait_until_F11_F12_released;
begin
  _last_debug_str_ := _debug_str_;
  _debug_str_ := 'ADT2KEYB.PAS:wait_until_key_released';
  Repeat
    realtime_gfx_poll_proc;
    draw_screen;
    keyboard_reset_buffer;
    If (inportb($60) > $80) then FillChar(keydown,SizeOf(keydown),0);
  until NOT keydown[$57] and NOT keydown[$58];
end;
