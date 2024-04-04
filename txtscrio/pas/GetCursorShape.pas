// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function GetCursorShape: Word;
{$IFDEF GO32V2}
var
  result: Word;

begin
  asm
        mov     dx,03d4h
        mov     al,0ah
        out     dx,al
        inc     dx
        in      al,dx
        and     al,1fh
        mov     ah,al
        dec     dx
        mov     al,0bh
        out     dx,al
        inc     dx
        in      al,dx
        and     al,1fh
        mov     result,ax
  end;
  GetCursorShape := result;
{$ELSE}
  GetCursorShape := virtual_cur_shape;
{$ENDIF}
end;
