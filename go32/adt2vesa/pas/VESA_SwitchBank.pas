// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure VESA_SwitchBank(Bank: Byte);

var
  regs: tRealRegs;
  granularity: Byte;

begin
  regs.ax := $4f05;
  regs.bx := 0;
  Case ModeInfoBlock.WinGranularity of
    32: granularity := 5;
    16: granularity := 4;
     8: granularity := 3;
     4: granularity := 2;
     2: granularity := 1;
     1: granularity := 0;
  end;
  regs.dx := bank SHL granularity;
  RealIntr($10,regs);
end;
