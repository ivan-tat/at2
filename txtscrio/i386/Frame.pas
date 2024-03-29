// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure Frame(dest: tSCREEN_MEM_PTR; x1,y1,x2,y2,atr1: Byte;
                title: String; atr2: Byte; border: String);
var
  xexp1,xexp2,xexp3,yexp1,yexp2: Byte;
  offs: Longint;

begin
  asm
        cmp     fr_setting.update_area,1
        jnz     @@0
        mov     al,x1
        mov     area_x1,al
        mov     al,y1
        mov     area_y1,al
        mov     al,x2
        mov     area_x2,al
        mov     al,y2
        mov     area_y2,al
@@0:    mov     bl,fr_setting.wide_range_type
        mov     bh,fr_setting.shadow_enabled
        lea     esi,[border]
        mov     edi,[dest]
        mov     offs,edi
        cmp     bl,0
        je      @@1
        mov     xexp1,4
        mov     xexp2,-1
        mov     xexp3,7
        mov     yexp1,1
        mov     yexp2,2
        jmp     @@2
@@1:    mov     xexp1,1
        mov     xexp2,2
        mov     xexp3,1
        mov     yexp1,0
        mov     yexp2,1
        jmp     @@4
@@2:    mov     al,x1
        sub     al,3
        mov     ah,y1
        dec     ah
        mov     dl,' '
        mov     dh,atr1
        xor     ecx,ecx
        mov     cl,x2
        sub     cl,x1
        add     cl,7
        call    DupChar
        mov     ah,y2
        inc     ah
        call    DupChar
        mov     bl,y1
@@3:    mov     al,x1
        sub     al,3
        mov     ah,bl
        mov     dl,' '
        mov     ecx,3
        call    DupChar
        mov     al,x2
        inc     al
        mov     dl,' '
        mov     ecx,3
        call    DupChar
        inc     bl
        cmp     bl,y2
        jng     @@3
@@4:    mov     al,x1
        mov     ah,y1
        mov     dl,[esi+1]
        mov     dh,atr1
        mov     ecx,1
        push    edi
        call    DupChar
        inc     al
        mov     dl,[esi+2]
        mov     dh,atr1
        mov     cl,x2
        sub     cl,x1
        dec     cl
        call    DupChar
        mov     al,x2
        mov     dl,[esi+3]
        mov     dh,atr1
        mov     ecx,1
        call    DupChar
        mov     bl,y1
@@5:    inc     bl
        mov     al,x1
        mov     ah,bl
        mov     dl,[esi+4]
        mov     dh,atr1
        mov     ecx,1
        call    DupChar
        inc     al
        mov     dl,' '
        mov     dh,atr1
        mov     cl,x2
        sub     cl,x1
        dec     cl
        call    DupChar
        mov     al,x2
        mov     dl,[esi+5]
        mov     dh,atr1
        mov     ecx,1
        call    DupChar
        cmp     bl,y2
        jnge    @@5
        mov     al,x1
        mov     ah,y2
        mov     dl,[esi+6]
        mov     dh,atr1
        mov     ecx,1
        call    DupChar
        inc     al
        mov     dl,[esi+7]
        mov     cl,x2
        sub     cl,x1
        dec     cl
        call    DupChar
        mov     al,x2
        mov     dl,[esi+8]
        mov     dh,atr1
        mov     ecx,1
        call    DupChar
        lea     esi,[title]
        mov     cl,[esi]
        jecxz   @@7
        xor     eax,eax
        mov     al,x2
        sub     al,x1
        sub     al,cl
        mov     bl,2
        div     bl
        add     al,x1
        add     al,ah
        mov     ah,y1
        xor     ecx,ecx
        call    DupChar
        push    eax
        xor     eax,eax
        mov     ax,absolute_pos
        mov     edi,offs
        add     edi,eax
        pop     eax
        lodsb
        mov     cl,al
        mov     ah,atr2
@@6:    lodsb
        stosw
        loop    @@6
@@7:    cmp     bh,0
        je      @@11
        mov     bl,y1
        sub     bl,yexp1
@@8:    inc     bl
        mov     al,x2
        add     al,xexp1
        mov     ah,bl
        xor     ecx,ecx
        call    DupChar
        push    eax
        xor     eax,eax
        mov     ax,absolute_pos
        mov     edi,offs
        add     edi,eax
        pop     eax
        inc     edi
        mov     al,07
        stosb
        inc     edi
        stosb
        cmp     MaxCol,180
        jna     @@9
        inc     edi
        stosb
@@9:    cmp     bl,y2
        jng     @@8
        mov     al,x1
        add     al,xexp2
        mov     ah,y2
        add     ah,yexp2
        xor     ecx,ecx
        call    DupChar
        push    eax
        xor     eax,eax
        mov     ax,absolute_pos
        mov     edi,offs
        add     edi,eax
        pop     eax
        inc     edi
        mov     al,07
        mov     cl,x2
        sub     cl,x1
        add     cl,xexp3
        cmp     MaxLn,60
        jb      @@10
        dec     cl
@@10:   stosb
        inc     edi
        loop    @@10
@@11:
  end;
end;
