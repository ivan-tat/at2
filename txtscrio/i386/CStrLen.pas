// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function CStrLen(str: String): Byte;

var
  result: Byte;

begin
  asm
        lea     esi,[str]
        lodsb
        xor     ebx,ebx
        xor     ecx,ecx
        mov     cl,al
        jecxz   @@3
@@1:    lodsb
        cmp     al,'~'
        jz      @@2
        inc     ebx
        loop    @@1
        jmp     @@3
@@2:    loop    @@1
@@3:    mov     eax,ebx
        mov     result,al
  end;
  CStrLen := result;
end;
