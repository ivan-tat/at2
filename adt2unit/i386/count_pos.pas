// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function count_pos(hpos: Byte): Byte;

var
  result: Byte;

begin
  asm
        mov     al,_pattedit_lastpos
        xor     ah,ah
        mov     bl,MAX_TRACKS
        div     bl
        mov     bl,al
        mov     al,hpos
        xor     ah,ah
        div     bl
        mov     al,ah
        dec     al
        or      ah,ah
        jnz     @@1
        dec     bl
        mov     al,bl
@@1:    mov     result,al
  end;
  count_pos := result;
end;
