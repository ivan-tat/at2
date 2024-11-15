// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure count_order(var entries: Byte);

var
  index,
  index2: Byte;

begin
  _dbg_enter ({$I %FILE%}, 'count_order');

  index := 0;
  index2 := 0;

  Repeat
    If (songdata.pattern_order[index] <> $80) then
      begin
        If (songdata.pattern_order[index] > $80) then
          If (songdata.pattern_order[index]-$80 <> index2) then
            begin
              index := songdata.pattern_order[index]-$80;
              index2 := index;
            end
          else BREAK;
      end
    else BREAK;
    If (index < $80) then Inc(index);
  until (index > $7f);

  entries := index;

  _dbg_leave; //EXIT //count_order
end;
