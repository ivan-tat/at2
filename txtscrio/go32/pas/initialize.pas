// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure initialize;
begin
  asm
        mov     ah,0fh
        int     10h
        and     al,7fh
        mov     v_mode,al
        mov     DispPg,bh
  end;

  MaxCol := MEM[SEG0040:$4a];
  MaxLn := SUCC(MEM[SEG0040:$84]);
  work_MaxLn  := MaxLn;
  work_MaxCol := MaxCol;
  FillWord(screen_ptr^,MAX_SCREEN_MEM_SIZE DIV 2,$0700);
  dosmemput(v_seg,v_ofs,screen_ptr^,MAX_SCREEN_MEM_SIZE);
end;
