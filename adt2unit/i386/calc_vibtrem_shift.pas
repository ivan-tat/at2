// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function calc_vibtrem_shift(chan: Byte; var table_data): Word;

var
  pos,dir,depth: Byte;
  result: Word;

begin
  pos := tVIBRATO_TREMOLO_TABLE(table_data)[chan].pos;
  depth := tVIBRATO_TREMOLO_TABLE(table_data)[chan].depth;
  asm
        xor     ebx,ebx
        mov     al,depth
        xor     ah,ah
        mov     bl,pos
        xor     bh,bh
        mov     dh,bl
        mov     cl,byte ptr [vibtrem_table_size]
        dec     cl
        and     bl,cl
        mov     dl,byte ptr [vibtrem_table+ebx]
        mul     dl
        rol     ax,1
        xchg    ah,al
        and     ah,1
        mov     dir,1
        mov     dl,byte ptr [vibtrem_table_size]
        test    dh,dl
        jne     @@1
        mov     dir,0
@@1:    mov     result,ax
  end;
  tVIBRATO_TREMOLO_TABLE(table_data)[chan].dir := dir;
  calc_vibtrem_shift := result;
end;
