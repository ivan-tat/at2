// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function ins_parameter(ins,param: Byte): Byte;

var
  result: Byte;

begin
  asm
        xor     ebx,ebx
        lea     esi,[songdata.instr_data]
        mov     bl,ins
        dec     ebx
        mov     eax,INSTRUMENT_SIZE
        mul     ebx
        add     esi,eax
        mov     bl,param
        add     esi,ebx
        lodsb
        mov     result,al
  end;
  ins_parameter := result;
end;
