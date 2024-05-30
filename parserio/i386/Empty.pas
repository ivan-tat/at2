// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function Empty(var buf; size: Longint): Boolean;

var
  result: Boolean;

begin
  asm
        xor     edx,edx
        mov     eax,size
        cmp     eax,16
        jb      @@3
        mov     ecx,4
        div     ecx
        mov     ecx,eax
        jecxz   @@1
        mov     edi,[buf]
        xor     eax,eax
        repz    scasd
        jnz     @@2
        mov     ecx,edx
        jecxz   @@1
        repz    scasb
        jnz     @@2
@@1:    mov     result,TRUE
        jmp     @@6
@@2:    mov     result,FALSE
        jmp     @@6
@@3:    mov     ecx,size
        jecxz   @@4
        mov     edi,[buf]
        xor     eax,eax
        repz    scasb
        jnz     @@5
@@4:    mov     result,TRUE
        jmp     @@6
@@5:    mov     result,FALSE
@@6:
  end;
  Empty := result;
end;
