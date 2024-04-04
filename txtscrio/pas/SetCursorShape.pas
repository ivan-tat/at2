// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure SetCursorShape(shape: Word);
begin
{$IFDEF GO32V2}
  asm
{$IFNDEF ADT2PLAY}
        mov     ax,shape
        mov     word ptr [virtual_cur_shape],ax
{$ENDIF}
        mov     dx,03d4h
        mov     al,0ah
        out     dx,al
        inc     dx
        in      al,dx
        mov     ah,byte ptr [shape+1]
        and     al,0e0h
        or      al,ah
        out     dx,al
        dec     dx
        mov     al,0bh
        out     dx,al
        inc     dx
        in      al,dx
        mov     ah,byte ptr [shape]
        and     al,0e0h
        or      al,ah
        out     dx,al
  end;
{$ELSE}
  virtual_cur_shape := shape;
{$ENDIF}
end;
