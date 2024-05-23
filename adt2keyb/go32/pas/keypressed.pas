// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function keypressed: Boolean;
begin
  realtime_gfx_poll_proc;
  draw_screen;
  // filter out CTRL+TAB combo as it is handled within timer routine
  If ctrl_tab_pressed then
    begin
      keyboard_reset_buffer;
      keypressed := FALSE;
    end
  else keypressed := CRT.KeyPressed;
end;
