// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure SetTextDisp(x,y: Word);

var
  maxcol_val: Byte;

begin
  While (inportb($3da) AND 1 = 1) do ;
  While (inportb($3da) AND 1 <> 1) do ;

  If NOT (program_screen_mode in [4,5]) then
    maxcol_val := MaxCol
  else maxcol_val := SCREEN_RES_X DIV scr_font_width;

  outportb($3d4,$0c);
  outportw($3d5,HI(WORD((y DIV scr_font_height)*maxcol_val+(x DIV scr_font_width))));
  outportb($3d4,$0d);
  outportw($3d5,LO(WORD((y DIV scr_font_height)*maxcol_val+(x DIV scr_font_width))));
  outportb($3d4,$08);
  outportb($3d5,(inportb($3d5) AND $0e0) OR (y AND $0f));
end;
