// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function _2nd_marked: Byte;

var
  temp,result: Byte;

begin
  result := 0;
  If (_1st_marked <> 0) then
    For temp := SUCC(_1st_marked) to 255 do
      If (songdata.instr_names[temp][1] = _ins_mark_chr) then
        begin
          result := temp;
          BREAK;
        end;
  _2nd_marked := result;
end;
