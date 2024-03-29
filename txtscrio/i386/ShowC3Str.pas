// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure ShowC3Str(dest: tSCREEN_MEM_PTR; x,y: Byte; str: String; atr1,atr2,atr3: Byte);
begin
  asm
        lea     esi,[str]
        mov     edi,dword ptr [dest]
        lodsb
        xor     ecx,ecx
        mov     cl,al
        jecxz   @@4
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
        mov     bl,atr2
        mov     bh,atr3
@@1:    lodsb
        cmp     al,'~'
        jz      @@2
        cmp     al,'`'
        jz      @@3
        stosw
        loop    @@1
        jmp     @@4
@@2:    xchg    ah,bl
        loop    @@1
        jmp     @@4
@@3:    xchg    ah,bh
        loop    @@1
@@4:
  end;
end;
