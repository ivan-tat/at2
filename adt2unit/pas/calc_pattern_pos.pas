// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function calc_pattern_pos(pattern: Byte): Byte;

var
  index: Integer;
  jump_count,pattern_pos: Byte;

begin
  _dbg_enter ({$I %FILE%}, 'calc_pattern_pos');

  pattern_pos := BYTE_NULL;
  jump_count := 0;
  index := calc_following_order(0);
  While (index <> -1) and (jump_count < $7f) do
    If (songdata.pattern_order[index] <> pattern) then
      If NOT (index < $7f) then BREAK
      else begin
             Inc(index);
             index := calc_following_order(index);
             Inc(jump_count);
           end
    else begin
           pattern_pos := index;
           BREAK;
         end;

  calc_pattern_pos := pattern_pos;

  _dbg_leave; //EXIT //calc_pattern_pos
end;
