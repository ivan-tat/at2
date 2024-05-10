// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function FilterStr1(str: String; chr0: Char): String;
begin
  asm
        lea     esi,[str]
        mov     edi,@RESULT
        mov     al,[esi]
        inc     esi
        inc     edi
        xor     ecx,ecx
        mov     cl,al
        mov     ebx,ecx
        jecxz   @@4
@@1:    mov     al,[esi]
        inc     esi
        cmp     al,chr0
        jnz     @@2
        dec     ebx
        jmp     @@3
@@2:    mov     [edi],al
        inc     edi
@@3:    loop    @@1
@@4:    mov     eax,ebx
        mov     edi,@RESULT
        mov     [edi],al
  end;
end;
