// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure VESA_GetPalette(var Palette); assembler;
asm
        mov     dx,3c7h
        xor     al,al
        out     dx,al
        mov     edi,Palette
        inc     dx
        inc     dx
        mov     ecx,768
        rep     insb
end;
