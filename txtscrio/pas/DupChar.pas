// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure PosChar(x,y: Byte);
begin
  absolute_pos := ((y-1)*MaxCol + x-1) shl 1;
end;

procedure DupChar(x,y: Byte; c: Char; attr,count: Byte; dest: Pointer);
begin
  absolute_pos := ((y-1)*MaxCol + x-1) shl 1; // exactly the same as in `PosChar'

  if (count <> 0) then
    FillWord ((dest+absolute_pos)^, count, Ord(c) + (attr shl 8));
end;
