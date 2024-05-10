// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function FilterStr1(str: String; chr0: Char): String;

var
  idx: Byte;
  result: String;

begin
  result := '';
  For idx := 1 to Length(str) do
    If (str[idx] <> chr0) then
      result := result+str[idx];
  FilterStr1 := result;
end;
