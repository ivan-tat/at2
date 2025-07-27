// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure NextCode; assembler;
asm
        mov     bx,le82a
        mov     ax,le82b
        add     bx,le78
        adc     ax,0
        xchg    bx,le82a
        xchg    ax,le82b
        mov     cx,bx
        and     cx,7
        shr     ax,1
        rcr     bx,1
        shr     ax,1
        rcr     bx,1
        shr     ax,1
        rcr     bx,1
        mov     esi,input_ptr
        mov     ax,[ebx+esi]
        mov     dl,[ebx+esi+2]
        or      cx,cx
        jz      @@2
@@1:    shr     dl,1
        rcr     ax,1
        loop    @@1
@@2:    mov     bx,le78
        sub     bx,9
        shl     bx,1
        and     ax,le7a[ebx]
end;

procedure LZW_decode;
begin
  asm
        xor     eax,eax
        xor     ebx,ebx
        xor     ecx,ecx
        mov     le72,0
        mov     le78,9
        mov     le70,102h
        mov     le74,200h
        mov     edi,output_ptr
        xor     eax,eax
        mov     le6a,ax
        mov     le6c,ax
        mov     le6e,ax
        mov     le76,al
        mov     le77,al
        mov     le82a,ax
        mov     le82b,ax
@@1:    call    NextCode
        cmp     ax,101h
        jnz     @@2
        jmp     @@9
@@2:    cmp     ax,100h
        jnz     @@3
        mov     le78,9
        mov     le74,200h
        mov     le70,102h
        call    NextCode
        mov     le6a,ax
        mov     le6c,ax
        mov     le77,al
        mov     le76,al
        mov     al,le77
        mov     byte ptr [edi],al
        inc     edi
        jmp     @@1
@@3:    mov     le6a,ax
        mov     le6e,ax
        cmp     ax,le70
        jb      @@4
        mov     ax,le6c
        mov     le6a,ax
        mov     al,le76
        push    eax
        inc     le72
@@4:    cmp     le6a,0ffh
        jbe     @@5
        mov     esi,work_ptr
        mov     bx,le6a
        shl     bx,1
        add     bx,le6a
        mov     al,[ebx+esi+2]
        push    eax
        inc     le72
        mov     ax,[ebx+esi]
        mov     le6a,ax
        jmp     @@4
@@5:    mov     ax,le6a
        mov     le76,al
        mov     le77,al
        push    eax
        inc     le72
        xor     ecx,ecx
        mov     cx,le72
        jecxz   @@7
@@6:    pop     eax
        mov     byte ptr [edi],al
        inc     edi
        loop    @@6
@@7:    mov     le72,0
        push    esi
        mov     bx,le70
        shl     bx,1
        add     bx,le70
        mov     esi,work_ptr
        mov     al,le77
        mov     [ebx+esi+2],al
        mov     ax,le6c
        mov     [ebx+esi],ax
        inc     le70
        pop     esi
        mov     ax,le6e
        mov     le6c,ax
        mov     bx,le70
        cmp     bx,le74
        jl      @@8
        cmp     le78,14
        jz      @@8
        inc     le78
        shl     le74,1
@@8:    jmp     @@1
@@9:    mov     eax,edi
        sub     eax,output_ptr
        mov     output_size,ax
  end;
end;
