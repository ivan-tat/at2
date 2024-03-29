// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure show_cstr(xpos,ypos: Byte; str: String; attr1,attr2: Byte);

var
  x11,x12,x21,x22,y11,y21: Byte;
  index,color1,color2: Byte;

begin
  asm
        mov     al,attr1
        mov     color1,al
        mov     al,attr2
        mov     color2,al
        xor     ebx,ebx
        xor     ecx,ecx
        mov     edi,dword ptr [screen_ptr]
        mov     edx,edi
        lea     esi,[str]
        lodsb
        mov     cl,al
        or      ecx,ecx
        jz      @@7
        mov     al,area_x1
        mov     x11,al
        inc     x11
        mov     x12,al
        add     x12,2
        mov     al,area_x2
        mov     x21,al
        inc     x21
        mov     x22,al
        add     x22,2
        mov     al,area_y1
        mov     y11,al
        inc     y11
        mov     al,area_y2
        mov     y21,al
        inc     y21
        mov     index,1
@@1:    mov     edi,edx
        xor     bx,bx
        mov     bl,xpos
        add     bl,index
        sub     bl,2
        mov     ah,ypos
        dec     ah
        mov     al,MaxCol
        mul     ah
        add     bx,ax
        shl     bx,1
        mov     al,xpos
        add     al,index
        dec     al
        mov     ah,ypos
        cmp     al,x12
        jnae    @@2
        cmp     al,x22
        jnbe    @@2
        cmp     ah,y21
        jne     @@2
        jmp     @@3
@@2:    cmp     al,x21
        jnae    @@4
        cmp     al,x22
        jnbe    @@4
        cmp     ah,y11
        jnae    @@4
        cmp     ah,y21
        jnbe    @@4
@@3:    add     edi,ebx
@@3a:   lodsb
        cmp     al,'~'
        jnz     @@3b
        push    eax
        mov     al,color1
        mov     ah,color2
        xchg    al,ah
        mov     color1,al
        mov     color2,ah
        pop     eax
        dec     cl
        cmp     index,cl
        jbe     @@3a
        cmp     al,'~'
        jz      @@7
@@3b:   stosb
        jmp     @@6
@@4:    cmp     al,area_x1
        jnae    @@5
        cmp     al,area_x2
        jnbe    @@5
        cmp     ah,area_y1
        jnae    @@5
        cmp     ah,area_y2
        jnbe    @@5
@@4a:   lodsb
        cmp     al,'~'
        jnz     @@6
        push    eax
        mov     al,color1
        mov     ah,color2
        xchg    al,ah
        mov     color1,al
        mov     color2,ah
        pop     eax
        dec     cl
        cmp     index,cl
        jbe     @@4a
        jmp     @@7
@@5:    add     edi,ebx
@@5a:   lodsb
        cmp     al,'~'
        jnz     @@5b
        push    eax
        mov     al,color1
        mov     ah,color2
        xchg    al,ah
        mov     color1,al
        mov     color2,ah
        pop     eax
        dec     cl
        cmp     index,cl
        jbe     @@5a
        jmp     @@7
@@5b:   mov     ah,color1
        stosw
@@6:    inc     index
        cmp     index,cl
        jbe     @@1
@@7:
  end;
end;
