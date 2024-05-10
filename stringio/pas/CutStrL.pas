// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function CutStrL(str: String; margin: Byte): String;

var
  idx: Byte;

begin
  If (margin = 0) then margin := Length(str)
  else If (margin > Length(str)) then
         margin := Length(str);
  idx := 0;
  While (idx+1 <= margin) and (str[idx+1] = ' ') do
    Inc(idx);
  If (idx <> 0) then Delete(str,1,idx);
  CutStrL := str;
end;
