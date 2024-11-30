// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function count_channel(hpos: Byte): Byte;

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
        or      ah,ah
        jz      @@1
        add     al,chan_pos
        jmp     @@2
@@1:    add     al,chan_pos
        dec     al
@@2:    mov     result,al
  end;
  count_channel := result;
end;
