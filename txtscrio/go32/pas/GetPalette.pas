// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure GetPalette(var pal; first,last: Word);
begin
  asm
        xor     eax,eax
        xor     ecx,ecx
        mov     ax,first
        mov     cx,last
        sub     ecx,eax
        inc     ecx
        mov     dx,03c7h
        out     dx,al
        add     dx,2
        mov     edi,[pal]
        add     edi,eax
        add     edi,eax
        add     edi,eax
        mov     eax,ecx
        add     ecx,eax
        add     ecx,eax
        rep     insb
  end;
end;
