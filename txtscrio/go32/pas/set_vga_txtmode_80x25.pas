// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure set_vga_txtmode_80x25;
begin
  asm
        mov     ax,03h
        xor     bh,bh
        int     10h
  end;

  v_seg := $0b800;
  v_ofs := 0;
  MaxCol := 80;
  MaxLn := 25;

  FillWord(screen_ptr^,MAX_SCREEN_MEM_SIZE DIV 2,$0700);
  dosmemput(v_seg,v_ofs,screen_ptr^,MAX_SCREEN_MEM_SIZE);
end;
