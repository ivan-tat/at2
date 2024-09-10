// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function detect_OPL3: Boolean;

var
  result: Boolean;

begin
  _last_debug_str_ := _debug_str_;
  _debug_str_ := 'ADT2OPL3.PAS:detect_OPL3';

  asm
        push    dword 04h
        push    dword 80h
        push    dword 04h
        push    dword 60h
        call    opl2out
        call    WaitRetrace
        call    opl2out
        call    WaitRetrace
        mov     dx,opl3port
        in      al,dx
        and     al,0e0h
        mov     bl,al
        push    dword 04h
        push    dword 21h
        push    dword 02h
        push    dword 0ffh
        call    opl2out
        call    WaitRetrace
        call    opl2out
        call    WaitRetrace
        mov     dx,opl3port
        in      al,dx
        and     al,0e0h
        mov     bh,al
        cmp     bx,0c000h
        jnz     @@1
        push    dword 04h
        push    dword 80h
        push    dword 04h
        push    dword 60h
        call    opl2out
        call    WaitRetrace
        call    opl2out
        call    WaitRetrace
        mov     dx,opl3port
        in      al,dx
        and     al,6
        or      al,al
        jnz     @@1
        mov     result,TRUE
        jmp     @@2
  @@1:  mov     result,FALSE
  @@2:
  end;

  detect_OPL3 := result;
end;
