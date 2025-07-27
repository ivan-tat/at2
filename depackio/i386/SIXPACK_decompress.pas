// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure InitTree;
begin
  asm
        xor     edi,edi
        mov     di,2
        mov     bx,2
        mov     cx,1
@@1:    xor     dx,dx
        mov     ax,di
        div     bx
        push    edi
        shl     di,1
        mov     word ptr dad[edi],ax
        mov     word ptr frq[edi],cx
        pop     edi
        inc     di
        cmp     di,TWICEMAX
        jbe     @@1
        mov     di,1
@@2:    xor     dx,dx
        mov     ax,di
        mul     bx
        push    edi
        shl     di,1
        mov     word ptr leftC[edi],ax
        inc     ax
        mov     word ptr rghtC[edi],ax
        pop     edi
        inc     di
        cmp     di,MAXCHAR
        jbe     @@2
  end;
end;

procedure UpdateFreq(a,b: Word);
begin
  asm
        xor     ecx,ecx
        xor     edi,edi
@@1:    mov     di,a
        shl     di,1
        mov     bx,word ptr frq[edi]
        mov     di,b
        shl     di,1
        add     bx,word ptr frq[edi]
        mov     di,a
        shl     di,1
        mov     dx,word ptr dad[edi]
        mov     di,dx
        shl     di,1
        mov     word ptr frq[edi],bx
        mov     a,dx
        cmp     a,ROOT
        jz      @@3
        mov     di,a
        shl     di,1
        mov     di,word ptr dad[edi]
        mov     ax,di
        shl     di,1
        mov     bx,word ptr leftC[edi]
        cmp     a,bx
        jnz     @@2
        mov     di,ax
        shl     di,1
        mov     bx,word ptr rghtC[edi]
        mov     b,bx
        jmp     @@3
@@2:    mov     di,ax
        shl     di,1
        mov     bx,word ptr leftC[edi]
        mov     b,bx
@@3:    cmp     a,ROOT
        jnz     @@1
        mov     bx,MAXFREQ
        mov     di,ROOT
        shl     di,1
        cmp     word ptr frq[edi],bx
        jnz     @@5
        lea     esi,[frq]
        lea     edi,[frq]
        mov     cx,TWICEMAX
        movsw
@@4:    lodsw
        shr     ax,1
        stosw
        loop    @@4
@@5:
  end;
end;

procedure UpdateModel(code: Word);
begin
  asm
        xor     ecx,ecx
        xor     edi,edi
        mov     bx,code
        add     bx,SUCCMAX
        mov     di,bx
        shl     di,1
        mov     ax,di
        mov     cx,word ptr frq[edi]
        inc     cx
        mov     word ptr frq[edi],cx
        mov     di,ax
        mov     cx,ROOT
        cmp     word ptr dad[edi],cx
        jz      @@10
        mov     dx,word ptr dad[edi]
        push    edi
        lea     edi,[leftC]
        mov     cx,dx
        shl     cx,1
        add     edi,ecx
        mov     si,word ptr [edi]
        pop     edi
        cmp     si,bx
        jnz     @@1
        mov     di,dx
        shl     di,1
        mov     si,word ptr rghtC[edi]
@@1:    push    ebx
        push    edx
        push    ebx
        push    esi
        call    UpdateFreq
        pop     edx
        pop     ebx
@@2:    xor     edi,edi
        mov     di,dx
        shl     di,1
        mov     ax,word ptr dad[edi]
        mov     di,ax
        shl     di,1
        mov     cx,di
        cmp     word ptr leftC[edi],dx
        jnz     @@3
        mov     di,cx
        mov     si,word ptr rghtC[edi]
        jmp     @@4
@@3:    mov     si,word ptr leftC[edi]
@@4:    xor     edi,edi
        mov     di,bx
        shl     di,1
        push    eax
        mov     ax,word ptr frq[edi]
        mov     di,si
        shl     di,1
        mov     cx,ax
        pop     eax
        cmp     cx,word ptr frq[edi]
        jbe     @@9
        mov     di,ax
        shl     di,1
        mov     cx,di
        cmp     word ptr leftC[edi],dx
        jnz     @@5
        mov     di,cx
        mov     word ptr rghtC[edi],bx
        jmp     @@6
@@5:    xor     edi,edi
        mov     di,cx
        mov     word ptr leftC[edi],bx
@@6:    lea     edi,[leftC]
        xor     ecx,ecx
        mov     cx,dx
        shl     cx,1
        add     edi,ecx
        cmp     word ptr [edi],bx
        jnz     @@7
        mov     word ptr [edi],si
        xor     edi,edi
        mov     di,cx
        mov     cx,word ptr rghtC[edi]
        jmp     @@8
@@7:    xor     edi,edi
        mov     di,cx
        mov     word ptr rghtC[edi],si
        mov     cx,word ptr leftC[edi]
@@8:    xor     edi,edi
        mov     di,si
        shl     di,1
        mov     word ptr dad[edi],dx
        mov     di,bx
        shl     di,1
        mov     word ptr dad[edi],ax
        push    esi
        push    esi
        push    ecx
        call    UpdateFreq
        pop     ebx
@@9:    xor     edi,edi
        mov     di,bx
        shl     di,1
        mov     bx,word ptr dad[edi]
        mov     di,bx
        shl     di,1
        mov     dx,word ptr dad[edi]
        cmp     dx,ROOT
        jnz     @@2
@@10:
  end;
end;

function InputCode(bits: Word): Word;

var
  result: Word;

begin
  asm
        xor     bx,bx
        xor     ecx,ecx
        mov     cx,1
@@1:    cmp     ibitCount,0
        jnz     @@3
        cmp     ibufCount,MAXBUF
        jnz     @@2
        mov     ax,input_size
        mov     ibufCount,0
@@2:    mov     edi,input_ptr
        xor     edx,edx
        mov     dx,ibufCount
        shl     dx,1
        add     edi,edx
        mov     ax,[edi]
        mov     ibitBuffer,ax
        inc     ibufCount
        mov     ibitCount,15
        jmp     @@4
@@3:    dec     ibitCount
@@4:    cmp     ibitBuffer,7fffh
        jbe     @@5
        xor     edi,edi
        mov     di,cx
        dec     di
        shl     di,1
        mov     ax,word ptr BitValue[edi]
        or      bx,ax
@@5:    shl     ibitBuffer,1
        inc     cx
        cmp     cx,bits
        jbe     @@1
        mov     ax,bx
        mov     result,ax
  end;
  InputCode := result;
end;

function Uncompress: Word;

var
  result: Word;

begin
  asm
        xor     eax,eax
        xor     ebx,ebx
        mov     bx,1
        mov     dx,ibitCount
        mov     cx,ibitBuffer
        mov     ax,ibufCount
@@1:    or      dx,dx
        jnz     @@3
        cmp     ax,MAXBUF
        jnz     @@2
        mov     ax,input_size
        xor     ax,ax
@@2:    shl     ax,1
        mov     edi,input_ptr
        add     edi,eax
        shr     ax,1
        mov     cx,[edi]
        inc     ax
        mov     dx,15
        jmp     @@4
@@3:    dec     dx
@@4:    cmp     cx,7fffh
        jbe     @@5
        mov     edi,ebx
        shl     edi,1
        mov     bx,word ptr rghtC[edi]
        jmp     @@6
@@5:    mov     edi,ebx
        shl     edi,1
        mov     bx,word ptr leftC[edi]
@@6:    shl     cx,1
        cmp     bx,MAXCHAR
        jle     @@1
        sub     bx,SUCCMAX
        mov     ibitCount,dx
        mov     ibitBuffer,cx
        mov     ibufCount,ax
        push    ebx
        push    ebx
        call    UpdateModel
        pop     eax
        mov     result,ax
  end;
  Uncompress := result;
end;

procedure SIXPACK_decode;
begin
  asm
        mov     ibitCount,0
        mov     ibitBuffer,0
        mov     obufCount,0
        mov     ibufCount,0
        xor     ebx,ebx
        xor     ecx,ecx
        mov     count,0
        call    InitTree
        call    Uncompress
@@1:    cmp     ax,TERMINATE
        jz      @@10
        cmp     ax,256
        jae     @@3
        mov     edi,output_ptr
        push    ebx
        mov     bx,obufCount
        add     edi,ebx
        pop     ebx
        stosb
        inc     obufCount
        mov     bx,MAXBUF
        cmp     obufCount,bx
        jnz     @@2
        mov     output_size,bx
        mov     obufCount,0
@@2:    mov     edi,work_ptr
        push    ebx
        mov     bx,count
        add     edi,ebx
        pop     ebx
        stosb
        inc     count
        cmp     count,MAXSIZE
        jnz     @@9
        mov     count,0
        jmp     @@9
@@3:    sub     ax,FIRSTCODE
        mov     cx,ax
        xor     dx,dx
        mov     bx,CODESPERRANGE
        div     bx
        mov     index,ax
        xor     dx,dx
        mul     bx
        mov     bx,cx
        add     bx,MINCOPY
        sub     bx,ax
        mov     si,bx
        xor     edi,edi
        mov     di,index
        shl     di,1
        mov     bx,word ptr CopyBits[edi]
        push    ebx
        call    InputCode
        add     ax,si
        xor     edi,edi
        mov     di,index
        shl     di,1
        add     ax,word ptr CopyMin[edi]
        mov     bx,count
        mov     dx,bx
        sub     dx,ax
        mov     cx,dx
        cmp     count,ax
        jae     @@4
        add     cx,MAXSIZE
@@4:    xor     dx,dx
@@5:    mov     edi,work_ptr
        add     edi,ecx
        mov     al,byte ptr [edi]
        mov     edi,output_ptr
        push    ebx
        mov     bx,obufCount
        add     edi,ebx
        pop     ebx
        mov     byte ptr [edi],al
        inc     obufCount
        mov     ax,MAXBUF
        cmp     obufCount,ax
        jnz     @@6
        mov     output_size,ax
        mov     obufCount,0
@@6:    mov     edi,work_ptr
        push    edi
        add     edi,ecx
        mov     al,byte ptr [edi]
        pop     edi
        add     edi,ebx
        mov     byte ptr [edi],al
        inc     bx
        cmp     bx,MAXSIZE
        jnz     @@7
        xor     bx,bx
@@7:    inc     cx
        cmp     cx,MAXSIZE
        jnz     @@8
        xor     cx,cx
@@8:    inc     dx
        cmp     dx,si
        jb      @@5
        mov     ax,si
        add     count,ax
        cmp     count,MAXSIZE
        jb      @@9
        sub     count,MAXSIZE
@@9:    call    Uncompress
        jmp     @@1
@@10:   mov     bx,obufCount
        mov     output_size,bx
  end;
end;
