// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

const
  _treat_char: array[$80..$a5] of Char =
    'CueaaaaceeeiiiAAE_AooouuyOU_____aiounN';

function FilterStr2(str: String; chr0: tCHARSET; chr1: Char): String;

var
  temp: Byte;

begin
  For temp := 1 to Length(str) do
    If NOT (str[temp] in chr0) then
      If (str[temp] >= #128) and (str[temp] <= #165) then
        str[temp] := _treat_char[BYTE(str[temp])]
      else If (str[temp] = #0) then str[temp] := ' '
           else str[temp] := chr1;
  FilterStr2 := str;
end;
