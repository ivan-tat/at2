// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function ReadChunk(str: String; pos: Byte): String;

var
  result: String;

begin
  Delete(str,1,pos-1);
  If (SYSTEM.Pos('$',str) = 0) then result := ''
  else result := Copy(str,1,SYSTEM.Pos('$',str)-1);
  ReadChunk := result;
end;
