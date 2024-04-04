// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function GetCursor: Longint;
{$IFDEF GO32V2}
var
  result: Longint;

begin
  asm
        xor     edx,edx
        mov     bh,DispPg
        mov     ah,03h
        int     10h
        shl     edx,16
        xor     eax,eax
        push    edx
        call    GetCursorShape
        pop     edx
        add     edx,eax
        mov     result,edx
  end;
  GetCursor := result;
{$ELSE}
  GetCursor := 0;
{$ENDIF}
end;
