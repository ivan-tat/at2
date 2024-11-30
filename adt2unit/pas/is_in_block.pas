// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function is_in_block(x0,y0,x1,y1: Byte): Boolean;
begin
  block_x1 := x1;
  block_x0 := block_xstart;
  If (block_x0 > block_x1) then
    begin
      block_x1 := block_x0;
      block_x0 := x1;
    end;

  block_y1 := y1;
  block_y0 := block_ystart;
  If (block_y0 > block_y1) then
    begin
      block_y1 := block_y0;
      block_y0 := y1;
    end;

  is_in_block := (x0 >= block_x0) and (x0 <= block_x1) and
                 (y0 >= block_y0) and (y0 <= block_y1);
end;
