// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure GotoXY(x,y: Byte);
begin
{$IFDEF GO32V2}
  asm
{$IFNDEF ADT2PLAY}
        lea     edi,[virtual_cur_pos]
        mov     ah,y
        mov     al,x
        stosw
{$ENDIF}
        mov     dh,y
        mov     dl,x
{$IFNDEF ADT2PLAY}
        add     dl,GOTOXY_xshift
{$ENDIF}
        dec     dh
        dec     dl
        mov     bh,DispPg
        mov     ah,02h
        int     10h
  end;
{$ELSE}
  virtual_cur_pos := x OR (y SHL 8);
{$ENDIF}
end;
