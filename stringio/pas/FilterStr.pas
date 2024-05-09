// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function FilterStr(str: String; chr0,chr1: Char): String;

var
  idx: Byte;

begin
  For idx := 1 to Length(str) do
    If (str[idx] = chr0) then
      str[idx] := chr1;
  FilterStr := str;
end;
