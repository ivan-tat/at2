// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function Update16(var buf; size: Longint; crc: Word): Word;

var
  result: Word;

begin
  asm
        mov     esi,[buf]
        lea     edi,[CRC16_table]
        mov     bx,crc
        mov     ecx,size
        jecxz   @@2
@@1:    xor     ax,ax
        lodsb
        mov     dl,bh
        xor     dh,dh
        xor     bh,bh
        xor     bx,ax
        and     ebx,000000ffh
        shl     ebx,1
        mov     bx,[edi+ebx]
        xor     bx,dx
        loop    @@1
@@2:    mov     ax,bx
        mov     result,ax
  end;
  Update16 := result;
end;
