// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure shift_text_screen;

var
  xsize: Byte;
  xshift: Byte;

begin
  xsize := SCREEN_RES_X DIV scr_font_width;
  xshift := (xsize-MAX_COLUMNS) DIV 2;
  FillChar(ptr_temp_screen2^,SCREEN_MEM_SIZE,0);
  asm
        mov     esi,dword ptr [screen_ptr]
        mov     edi,dword ptr [ptr_temp_screen2]
        cld
        movzx   ecx,MAX_ROWS
        movzx   ebx,xshift
        shl     ebx,1
        add     edi,ebx
@@1:    xchg    ecx,edx
        movzx   ecx,xsize
        rep     movsw
        xchg    ecx,edx
        loop    @@1
  end;
end;
