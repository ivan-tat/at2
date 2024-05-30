// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function Scan(var buf; skip,size: Longint; str: String): Longint;

var
  result: Longint;

begin
  asm
        lea     edi,[str]
        mov     esi,edi
        xor     eax,eax
        lodsb
        stosb
        xor     ecx,ecx
        mov     ecx,eax
        xor     ebx,ebx
        mov     ebx,eax
        jecxz    @@9
@@1:    lodsb
        cmp     al,'a'
        jb      @@2
        cmp     al,'z'
        ja      @@2
        sub     al,20h
@@2:    stosb
        loop    @@1
        sub     edi,ebx
        mov     esi,[buf]
        add     esi,skip
        mov     ecx,size
        sub     ecx,skip
        jecxz   @@8
        cld
        sub     ecx,ebx
        jb      @@8
        inc     ecx
@@4:    mov     ah,[edi]
        and     ah,NOT 20h
@@5:    lodsb
        and     al,NOT 20h
        cmp     al,ah
        loopne  @@5
        jne     @@8
        dec     esi
        mov     edx,ecx
        mov     ecx,ebx
@@6:    repe    cmpsb
        je      @@10
        mov     al,[esi-1]
        cmp     al,'a'
        jb      @@7
        cmp     al,'z'
        ja      @@7
        sub     al,20h
@@7:    cmp     al,[edi-1]
        je      @@6
        sub     ecx,ebx
        add     esi,ecx
        add     edi,ecx
        inc     esi
        mov     ecx,edx
        jne     @@4
@@8:    xor     eax,eax
        jmp     @@11
@@9:    mov     eax,1
        jmp     @@11
@@10:   sub     esi,ebx
        mov     eax,esi
        sub     eax,dword ptr [buf]
        inc     eax
@@11:   dec     eax
        mov     result,eax
  end;
  Scan := result;
end;
