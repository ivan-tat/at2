// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure LoadVgaRegisters(reg: VGA_REG_DATA);

var
  idx,temp: Byte;

begin
  outportb($3d4,$11);
  temp := inportb($3d5) AND $7f;
  outportb($3d4,$11);
  outportb($3d5,temp);
  For idx := 1 to 29 do out_reg(reg[idx]);
end;
