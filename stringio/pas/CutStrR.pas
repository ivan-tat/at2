// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function CutStrR(str: String; margin: Byte): String;

var
  idx: Byte;

begin
  If (margin > Length(str)) then
    margin := Length(str);
  idx := 0;
  While (str[BYTE(str[0])-idx] = ' ') and
        (BYTE(str[0])-idx >= margin) do
    Inc(idx);
  Dec(BYTE(str[0]),idx);
  CutStrR := str;
end;
