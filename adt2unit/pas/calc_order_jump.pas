// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function calc_order_jump: Integer;

var
  temp: Byte;
  result: Integer;

begin
  _dbg_enter ({$I %FILE%}, 'calc_order_jump');

  result := 0;
  temp := 0;

  Repeat
    If (songdata.pattern_order[current_order] > $7f) then
      current_order := songdata.pattern_order[current_order]-$80;
    Inc(temp);
  until (temp > $7f) or (songdata.pattern_order[current_order] < $80);

  If (temp > $7f) then begin stop_playing; result := -1; end;
  calc_order_jump := result;

  _dbg_leave; //EXIT //calc_order_jump
end;
