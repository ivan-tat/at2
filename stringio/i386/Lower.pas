// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function Lower(str: String): String;
begin
  asm
        lea     esi,[str]
        mov     edi,@RESULT
        mov     al,[esi]
        inc     esi
        mov     [edi],al
        inc     edi
        xor     ecx,ecx
        mov     cl,al
        jecxz   @@3
@@1:    mov     al,[esi]
        inc     esi
        cmp     al,'A'
        jb      @@2
        cmp     al,'Z'
        ja      @@2
        add     al,20h
@@2:    mov     [edi],al
        inc     edi
        loop    @@1
@@3:
  end;
end;
