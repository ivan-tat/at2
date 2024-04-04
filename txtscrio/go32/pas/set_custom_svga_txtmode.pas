//  This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure set_custom_svga_txtmode;
begin
  LoadVgaRegisters(svga_txtmode_regs);
  MaxCol := svga_txtmode_cols;
  MaxLn := svga_txtmode_rows;
  MEM[SEG0040:$4a] := MaxCol;
  MEM[SEG0040:$84] := MaxLn-1;
  initialize;
  dosmemput(v_seg,v_ofs,screen_ptr^,MAX_SCREEN_MEM_SIZE);
end;
