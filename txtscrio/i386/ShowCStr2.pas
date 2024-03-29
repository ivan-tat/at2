// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure ShowCStr2(dest: tSCREEN_MEM_PTR; x,y: Byte; str: String; atr1,atr2: Byte);
begin
  asm
        lea     esi,[str]
        mov     edi,dword ptr [dest]
        lodsb
        xor     ecx,ecx
        mov     cl,al
        jecxz   @@3
        push    ecx
        mov     al,x
        mov     ah,y
        xor     ecx,ecx
        call    DupChar
        xor     edx,edx
        mov     dx,absolute_pos
        pop     ecx
        add     edi,edx
        mov     ah,atr1
        mov     bh,atr2
@@1:    lodsb
        cmp     al,'"'
        jz      @@2
        stosw
        loop    @@1
        jmp     @@3
@@2:    xchg    ah,bh
        loop    @@1
@@3:
  end;
end;
