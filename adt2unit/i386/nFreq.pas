// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function nFreq(note: Byte): Word;

const
  Fnum: array[0..11] of Word = (
    $157,$16b,$181,$198,$1b0,$1ca,$1e5,$202,$220,$241,$263,$287);

var
  result: Word;

begin
  asm
        xor     ebx,ebx
        mov     al,[note]
        xor     ah,ah
        cmp     ax,12*8
        jae     @@1
        push    eax
        mov     bl,12
        div     bl
        mov     bl,ah
        xor     bh,bh
        shl     bx,1
        pop     eax
        mov     cl,12
        div     cl
        xor     ah,ah
        shl     ax,10
        add     ax,word ptr [Fnum+ebx]
        jmp     @@2
@@1:    mov     ax,7
        shl     ax,10
        add     ax,FreqEnd
@@2:    mov     result,ax
  end;
  nFreq := result;
end;
