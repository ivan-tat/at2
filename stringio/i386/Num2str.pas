// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function Num2str(num: Longint; base: Byte): String;

const
  hexa: array[0..PRED(16)+32] of Char = '0123456789ABCDEF'+
                                        #0#0#0#0#0#0#0#0#0#0#0#0#0#0#0#0;
begin
  asm
        xor     eax,eax
        xor     edx,edx
        xor     edi,edi
        xor     esi,esi
        mov     eax,num
        xor     ebx,ebx
        mov     bl,base
        cmp     bl,2
        jb      @@3
        cmp     bl,16
        ja      @@3
        mov     edi,32
@@1:    dec     edi
        xor     edx,edx
        div     ebx
        mov     esi,edx
        mov     dl,byte ptr [hexa+esi]
        mov     byte ptr [hexa+edi+16],dl
        and     eax,eax
        jnz     @@1
        mov     esi,edi
        mov     ecx,32
        sub     ecx,edi
        mov     edi,@RESULT
        mov     al,cl
        stosb
@@2:    mov     al,byte ptr [hexa+esi+16]
        stosb
        inc     esi
        loop    @@2
        jmp     @@4
@@3:    mov     edi,@RESULT
        xor     al,al
        stosb
@@4:
  end;
end;
