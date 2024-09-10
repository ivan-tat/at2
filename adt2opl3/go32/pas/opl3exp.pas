// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure opl3exp(data: Word);
begin
  if (_opl_regs_cache[(data AND $ff) OR $100] <> data SHR 8) then
    _opl_regs_cache[(data AND $ff) OR $100] := data SHR 8
  else EXIT;

  asm
        mov     ax,data
        mov     dx,word ptr [opl3port]
        add     dx,2
        out     dx,al
        mov     ecx,6
@@1:    in      al,dx
        loop    @@1
        inc     dl
        mov     al,ah
        out     dx,al
        mov     ecx,36
@@2:    in      al,dx
        loop    @@2
  end;
end;
