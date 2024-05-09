// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function byte2dec(value: Byte): String;

const
  data: array[0..9] of char = '0123456789';

begin
  asm
        mov     edi,@RESULT
        lea     ebx,[data]
        mov     al,value
        xor     ah,ah
        mov     cl,100
        div     cl
        mov     ch,ah
        xchg    ah,al
        or      ah,ah
        jz      @@1
        mov     al,3
        stosb
        xchg    ah,al
        xlat
        stosb
        mov     al,ch
        jmp     @@2
@@1:    mov     al,2
        stosb
        mov     al,value
@@2:    xor     ah,ah
        mov     cl,10
        div     cl
        xlat
        stosb
        mov     al,ah
        xlat
        stosb
  end;
end;
