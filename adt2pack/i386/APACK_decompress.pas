// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function APACK_decompress(var source,dest): Dword;

var
  temp,result: Dword;

begin
  asm
        mov     esi,[source]
        mov     edi,[dest]
        cld
        mov     dl,80h
@@1:    movsb
@@2:    add     dl,dl
        jnz     @@3
        mov     dl,[esi]
        inc     esi
        adc     dl,dl
@@3:    jnc     @@1
        xor     ecx,ecx
        add     dl,dl
        jnz     @@4
        mov     dl,[esi]
        inc     esi
        adc     dl,dl
@@4:    jnc     @@8
        xor     eax,eax
        add     dl,dl
        jnz     @@5
        mov     dl,[esi]
        inc     esi
        adc     dl,dl
@@5:    jnc     @@15
        inc     ecx
        mov     al,10h
@@6:    add     dl,dl
        jnz     @@7
        mov     dl,[esi]
        inc     esi
        adc     dl,dl
@@7:    adc     al,al
        jnc     @@6
        jnz     @@24
        stosb
        jmp     @@2
@@8:    inc     ecx
@@9:    add     dl,dl
        jnz     @@10
        mov     dl,[esi]
        inc     esi
        adc     dl,dl
@@10:   adc     ecx,ecx
        add     dl,dl
        jnz     @@11
        mov     dl,[esi]
        inc     esi
        adc     dl,dl
@@11:   jc      @@9
        dec     ecx
        loop    @@16
        xor     ecx,ecx
        inc     ecx
@@12:   add     dl,dl
        jnz     @@13
        mov     dl,[esi]
        inc     esi
        adc     dl,dl
@@13:   adc     ecx,ecx
        add     dl,dl
        jnz     @@14
        mov     dl,[esi]
        inc     esi
        adc     dl,dl
@@14:   jc      @@12
        jmp     @@23
@@15:   lodsb
        shr     eax,1
        jz      @@25
        adc     ecx,ecx
        jmp     @@20
@@16:   xchg    eax,ecx
        dec     eax
        shl     eax,8
        lodsb
        xor     ecx,ecx
        inc     ecx
@@17:   add     dl,dl
        jnz     @@18
        mov     dl,[esi]
        inc     esi
        adc     dl,dl
@@18:   adc     ecx,ecx
        add     dl,dl
        jnz     @@19
        mov     dl,[esi]
        inc     esi
        adc     dl,dl
@@19:   jc      @@17
        cmp     eax,32000
        jae     @@20
        cmp     ah,5
        jae     @@21
        cmp     eax,7fh
        ja      @@22
@@20:   inc     ecx
@@21:   inc     ecx
@@22:   xchg    eax,temp
@@23:   mov     eax,temp
@@24:   push    esi
        mov     esi,edi
        sub     esi,eax
        rep     movsb
        pop     esi
        jmp     @@2
@@25:   sub     edi,[dest]
        mov     result,edi
  end;
  APACK_decompress := result;
end;
