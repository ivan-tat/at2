// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure GetChar; assembler;
asm
        push    ebx
        mov     bx,ibufCount
        cmp     bx,ibufSize
        jb      @@1
        jmp     @@2
@@1:    push    edi
        mov     edi,input_ptr
        mov     al,byte ptr [edi+ebx]
        pop     edi
        inc     ebx
        mov     ibufCount,bx
        pop     ebx
        clc
        jmp     @@3
@@2:    pop     ebx
        stc
@@3:
end;

procedure PutChar; assembler;
asm
        push    ebx
        mov     bx,output_size
        push    edi
        mov     edi,output_ptr
        mov     byte ptr [edi+ebx],al
        pop     edi
        inc     ebx
        mov     output_size,bx
        pop     ebx
end;

procedure LZSS_decode;
begin
  asm
        mov     ibufCount,0
        mov     ax,input_size
        mov     ibufSize,ax
        mov     output_size,0
        xor     ebx,ebx
        xor     edx,edx
        mov     edi,N-F
@@1:    shr     dx,1
        or      dh,dh
        jnz     @@2
        call    GetChar
        jc      @@5
        mov     dh,0ffh
        mov     dl,al
@@2:    test    dx,1
        jz      @@3
        call    GetChar
        jc      @@5
        push    esi
        mov     esi,work_ptr
        add     esi,edi
        mov     byte ptr [esi],al
        pop     esi
        inc     edi
        and     edi,N-1
        call    PutChar
        jmp     @@1
@@3:    call    GetChar
        jc      @@5
        mov     ch,al
        call    GetChar
        jc      @@5
        mov     bh,al
        mov     cl,4
        shr     bh,cl
        mov     bl,ch
        mov     cl,al
        and     cl,0fh
        add     cl,THRESHOLD
        inc     cl
@@4:    and     ebx,N-1
        push    esi
        mov     esi,work_ptr
        mov     al,byte ptr [esi+ebx]
        add     esi,edi
        mov     byte ptr [esi],al
        pop     esi
        inc     edi
        and     edi,N-1
        call    PutChar
        inc     ebx
        dec     cl
        jnz     @@4
        jmp     @@1
@@5:
  end;
end;
