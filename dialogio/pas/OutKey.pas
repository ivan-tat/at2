// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function OutKey(str: String): Char;

var
  result: Char;

begin
  If (SYSTEM.Pos('~',str) = 0) then result := '~'
  else If (str[SYSTEM.Pos('~',str)+2] <> '~') then result := '~'
       else result := str[SYSTEM.Pos('~',str)+1];
  OutKey := result;
end;
