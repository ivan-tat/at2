// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function SensitiveScan(var buf; skip,size: Longint; str: String): Longint;

var
  result: Longint;

begin
  asm
        mov     edi,[buf]
        add     edi,skip
        lea     esi,[str]
        mov     ecx,size
        sub     ecx,skip
        xor     eax,eax
        jecxz   @@3
        cld
        lodsb
        cmp     al,1
        jb      @@5
        ja      @@1
        lodsb
        repne   scasb
        jne     @@3
        jmp     @@5
@@1:    xor     ah,ah
        mov     ebx,eax
        dec     ebx
        mov     edx,ecx
        sub     edx,eax
        jb      @@3
        lodsb
        add     edx,2
@@2:    dec     edx
        mov     ecx,edx
        repne   scasb
        jne     @@3
        mov     edx,ecx
        mov     ecx,ebx
        rep     cmpsb
        je      @@4
        sub     ecx,ebx
        add     esi,ecx
        add     edi,ecx
        inc     edi
        or      edx,edx
        jne     @@2
@@3:    xor     eax,eax
        jmp     @@6
@@4:    sub     edi,ebx
@@5:    mov     eax,edi
        sub     eax,dword ptr [buf]
@@6:    dec     eax
        mov     result,eax
  end;
  SensitiveScan := result;
end;
