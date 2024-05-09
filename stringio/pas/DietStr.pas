// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function DietStr(str: String; size: Byte): String;
begin
  If (Length(str) <= size) then
    begin
      DietStr := str;
      EXIT;
    end;

  Repeat
    Delete(str,size DIV 2,1)
  until (Length(str)+3 = size);

  Insert('...',str,size DIV 2);
  DietStr := str
end;
