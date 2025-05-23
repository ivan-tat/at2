// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure opl2out(reg,data: Word);
begin
  If (_opl_regs_cache[reg] <> data) then
    _opl_regs_cache[reg] := data
  else
    EXIT; //opl2out

  asm
        mov     ax,reg
        mov     dx,word ptr [opl3port]
        or      ah,ah
        jz      @@1
        add     dx,2
@@1:    out     dx,al
        mov     ecx,6
@@2:    in      al,dx
        loop    @@2
        inc     dl
        mov     ax,data
        out     dx,al
        dec     dl
        mov     ecx,36
@@3:    in      al,dx
        loop    @@3
  end;

  //EXIT //opl2out
end;
