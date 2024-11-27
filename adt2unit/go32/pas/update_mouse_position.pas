// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure update_mouse_position;
var
  regs: tRealRegs;
begin
  If mouse_active then
    begin
      FillChar(regs,SizeOf(regs),0);
      regs.ax := $04;
      regs.cx := scr_scroll_x;
      regs.dx := scr_scroll_y;
      RealIntr($33,regs);
    end;
end;
