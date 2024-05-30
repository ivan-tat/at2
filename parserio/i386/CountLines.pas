// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function CountLines(var buf; size: Longint): Longint;

var
  result: Longint;

begin
  asm
        mov     edi,[buf]
        mov     ecx,size
        mov     edx,edi
        add     edx,ecx
        xor     ebx,ebx
        jecxz   @@3
@@1:    mov     al,0dh
        repnz   scasb
        jnz     @@3
        cmp     byte ptr [edi],0ah
        jnz     @@2
        inc     edi
        inc     ebx
@@2:    cmp     edi,edx
        jb      @@1
@@3:    mov     eax,ebx
        mov     result,eax
  end;
  CountLines := result;
end;
