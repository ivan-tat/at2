// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure SetPalette(var pal; first,last: Word);
begin
  asm
        mov     dx,03dah
@@1:    in      al,dx
        test    al,8
        jz      @@1
        xor     eax,eax
        xor     ecx,ecx
        mov     ax,first
        mov     cx,last
        sub     ecx,eax
        inc     ecx
        mov     dx,03c8h
        out     dx,al
        inc     dx
        mov     esi,[pal]
        add     esi,eax
        add     esi,eax
        add     esi,eax
        mov     eax,ecx
        add     ecx,eax
        add     ecx,eax
        rep     outsb
  end;
end;
