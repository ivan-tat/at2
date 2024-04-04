// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure SplitScr(line: Word);

var
  temp: Byte;

begin
  outportb($3d4,$18);
  outportb($3d5,LO(line));
  outportb($3d4,$07);
  temp := inportb($3d5);

  If (line < $100) then temp := temp AND $0ef
  else temp := temp OR $10;

  outportb($3d5,temp);
  outportb($3d4,$09);
  temp := inportb($3d5);

  If (line < $200) then temp := temp AND $0bf
  else temp := temp OR $40;

  outportb($3d5,temp);
end;
