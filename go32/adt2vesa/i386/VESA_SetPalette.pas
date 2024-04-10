// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure VESA_SetPalette(var Palette); assembler;
asm
        mov     dx,3dah
@@1:    in      al,dx
        test    al,8
        jz      @@1
        mov     dx,3c8h
        xor     al,al
        out     dx,al
        mov     esi,Palette
        inc     dx
        mov     ecx,768
        rep     outsb
end;
