// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure ShowStr(dest: tSCREEN_MEM_PTR; x,y: Byte; str: String; attr: Byte);
begin
  asm
        mov     edi,dword ptr [dest]
        lea     esi,[str]
        mov     al,x
        mov     ah,y
        xor     ecx,ecx
        call    DupChar
        xor     edx,edx
        mov     dx,absolute_pos
        lodsb
        mov     cl,al
        jecxz   @@2
        add     edi,edx
        mov     ah,attr
@@1:    lodsb
        stosw
        loop    @@1
@@2:
  end;
end;
