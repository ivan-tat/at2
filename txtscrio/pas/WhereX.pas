// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function WhereX: Byte;
{$IFDEF GO32V2}
var
  result: Byte;

begin
  asm
        mov     bh,DispPg
        mov     ah,03h
        int     10h
        inc     dl
        mov     result,dl
  end;
  WhereX := result;
{$ELSE}
  WhereX := virtual_cur_pos AND $0ff;
{$ENDIF}
end;
