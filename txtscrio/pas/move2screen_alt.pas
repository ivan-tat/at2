// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure move2screen_alt;

var
  a,b: Pointer;
  x,y: Byte;

begin
  if (move_to_screen_data <> NIL) then begin
    Move (screen_ptr^, temp_screen2, SCREEN_MEM_SIZE);

    a := move_to_screen_data;
    b := ptr_temp_screen2;

    for y := move_to_screen_area[2] to move_to_screen_area[4] do begin
      for x := move_to_screen_area[1] to move_to_screen_area[3] do begin
        PosChar (x, y);
        Word((b+absolute_pos)^) := Word((a+absolute_pos)^);
      end;
    end;

    Move (temp_screen2, screen_ptr^, SCREEN_MEM_SIZE);
  end;
end;
