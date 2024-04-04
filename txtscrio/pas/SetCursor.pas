// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure SetCursor(cursor: Longint);
begin
{$IFDEF GO32V2}
  asm
{$IFNDEF ADT2PLAY}
        lea     edi,[virtual_cur_pos]
        mov     ax,word ptr [cursor+2]
        stosw
{$ENDIF}
        xor     eax,eax
        mov     ax,word ptr [cursor]
        push    eax
        call    SetCursorShape
        mov     dx,word ptr [cursor+2]
        mov     bh,DispPg
        mov     ah,02h
        int     10h
  end;
{$ELSE}
  virtual_cur_pos := cursor SHR 16;
  SetCursorShape(cursor AND WORD_NULL);
{$ENDIF}
end;
