// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function calc_following_order(order: Byte): Integer;

var
  result: Integer;
  index,jump_count: Byte;

begin
  _dbg_enter ({$I %FILE%}, 'calc_following_order');

  result := -1;
  index := order;
  jump_count := 0;

  Repeat
    If (songdata.pattern_order[index] < $80) then result := index
    else begin
           index := songdata.pattern_order[index]-$80;
           Inc(jump_count);
         end;
  until (jump_count > $7f) or
        (result <> -1);

  calc_following_order := result;

  _dbg_leave; //EXIT //calc_following_order
end;
