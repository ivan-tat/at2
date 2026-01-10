// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function _1st_marked: Byte;

var
  temp,result: Byte;

begin
  result := 0;
  For temp := 1 to 255 do
    If (songdata.instr_names[temp][1] = _ins_mark_chr) then
      begin
        result := temp;
        BREAK;
      end;
  _1st_marked := result;
end;
