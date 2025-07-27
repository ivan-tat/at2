// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure RDC_decode;
begin
  asm
        mov     ctrl_mask,0
        mov     eax,input_ptr
        mov     ibuf_end,eax
        xor     eax,eax
        mov     ax,input_size
        add     ibuf_end,eax
        mov     eax,input_ptr
        mov     ibuf_idx,eax
        mov     eax,output_ptr
        mov     obuf_idx,eax
@@1:    xor     ecx,ecx
        mov     eax,ibuf_idx
        cmp     eax,ibuf_end
        jnb     @@7
        mov     ax,ctrl_mask
        shr     ax,1
        mov     ctrl_mask,ax
        or      ax,ax
        jnz     @@2
        mov     esi,ibuf_idx
        lodsw
        mov     ctrl_bits,ax
        add     ibuf_idx,2
        mov     ctrl_mask,8000h
@@2:    mov     ax,ctrl_bits
        and     ax,ctrl_mask
        or      ax,ax
        jnz     @@3
        mov     esi,ibuf_idx
        mov     edi,obuf_idx
        movsb
        inc     ibuf_idx
        inc     obuf_idx
        jmp     @@1
@@3:    xor     ah,ah
        mov     esi,ibuf_idx
        lodsb
        shr     ax,4
        and     ax,0fh
        mov     command,ax
        xor     ah,ah
        mov     esi,ibuf_idx
        lodsb
        and     ax,0fh
        mov     count,ax
        inc     ibuf_idx
        cmp     command,0
        jnz     @@4
        add     count,3
        mov     edi,obuf_idx
        mov     cx,count
        mov     esi,ibuf_idx
        lodsb
        rep     stosb
        inc     ibuf_idx
        mov     cx,count
        add     obuf_idx,ecx
        jmp     @@1
@@4:    cmp     command,1
        jnz     @@5
        xor     ah,ah
        mov     esi,ibuf_idx
        lodsb
        shl     ax,4
        add     count,ax
        inc     ibuf_idx
        add     count,19
        mov     edi,obuf_idx
        mov     cx,count
        mov     esi,ibuf_idx
        lodsb
        rep     stosb
        inc     ibuf_idx
        mov     cx,count
        add     obuf_idx,ecx
        jmp     @@1
@@5:    cmp     command,2
        jnz     @@6
        mov     ax,count
        add     ax,3
        mov     offs,ax
        xor     ah,ah
        mov     esi,ibuf_idx
        lodsb
        shl     ax,4
        add     offs,ax
        inc     ibuf_idx
        xor     ah,ah
        mov     esi,ibuf_idx
        lodsb
        mov     count,ax
        inc     ibuf_idx
        add     count,16
        mov     eax,obuf_idx
        mov     cx,offs
        sub     eax,ecx
        mov     obuf_src,eax
        mov     esi,eax
        mov     edi,obuf_idx
        mov     cx,count
        rep     movsb
        mov     cx,count
        add     obuf_idx,ecx
        jmp     @@1
@@6:    mov     ax,count
        add     ax,3
        mov     offs,ax
        xor     ah,ah
        mov     esi,ibuf_idx
        lodsb
        shl     ax,4
        add     offs,ax
        inc     ibuf_idx
        mov     eax,obuf_idx
        mov     cx,offs
        sub     eax,ecx
        mov     obuf_src,eax
        mov     esi,eax
        mov     edi,obuf_idx
        mov     cx,command
        rep     movsb
        mov     cx,command
        add     obuf_idx,ecx
        jmp     @@1
@@7:    mov     eax,obuf_idx
        sub     eax,output_ptr
        mov     output_size,ax
  end;
end;
