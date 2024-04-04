// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure ResetMode;
begin
  asm
        xor     ah,ah
        mov     al,v_mode
        mov     bh,DispPg
        int     10h
  end;
  v_seg := $0b800;
  v_ofs := 0;
  MaxCol := MEM[SEG0040:$4a];
  MaxLn := SUCC(MEM[SEG0040:$84]);
  FillWord(screen_ptr^,MAX_SCREEN_MEM_SIZE DIV 2,$0700);
  dosmemput(v_seg,v_ofs,screen_ptr^,MAX_SCREEN_MEM_SIZE);
end;
