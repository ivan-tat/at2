// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function CenterStr(str: String; size: Byte): String;

var
  flag: Boolean;

begin
  flag := FALSE;
  While (Length(str) < size) do
    begin
      If flag then
        str := ' ' + str
      else str := str + ' ';
      flag := NOT flag;
    end;
  CenterStr := str;
end;
