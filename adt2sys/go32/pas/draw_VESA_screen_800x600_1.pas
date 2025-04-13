// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure draw_VESA_screen_800x600_1;

const
  H_RES = 800;
  V_RES = 600;
  H_CHR = 90;
  V_CHR = 30;

var
  byte_pos,bit_mask: Byte;
  cur_pos_lo,cur_pos_hi: Byte;
  cur_shape: Word;
  pos_x,pos_y: Byte;
  base_ofs,skip_ofs: Dword;
  loop_idx1,loop_idx2,loop_idx3,
  loop_idx4: Dword;

begin
  asm
        push    ebx
        push    esi
        push    edi
        mov     ax,word ptr [virtual_cur_pos]
        mov     cur_pos_lo,al
        mov     cur_pos_hi,ah
        mov     ax,word ptr [virtual_cur_shape]
        mov     cur_shape,ax
        mov     eax,_cursor_blink_factor
        cmp     _cursor_blink_pending_frames,eax
        jnae    @@1
        mov     _cursor_blink_pending_frames,0
        xor     byte ptr [cursor_sync],1
@@1:    lea     esi,[font8x16]
        lea     edi,[_FrameBuffer_mirror]
        mov     base_ofs,edi
        add     edi,(H_RES-H_CHR*8)/2+(V_RES-V_CHR*16)/2*H_RES
        mov     ebx,dword ptr [screen_ptr]
        mov     eax,virtual_screen__first_row
        mov     skip_ofs,eax
        movzx   eax,MAX_ROWS
        mov     loop_idx1,eax
        mov     pos_y,1
@@2:    mov     byte_pos,0
        mov     loop_idx2,16
@@3:    mov     loop_idx3,H_CHR
        mov     pos_x,1
@@4:    movzx   eax,byte ptr [ebx]
        mov     edx,16
        mul     edx
        movzx   edx,byte_pos
        add     eax,edx
        mov     dl,[esi+eax]
        mov     bit_mask,dl
        mov     loop_idx4,8
@@5:    mov     edx,1
        mov     ecx,loop_idx4
        shl     dx,cl
        shr     dx,1
        cmp     skip_ofs,0
        jz      @@6
        dec     skip_ofs
        jmp     @@9
@@6:    mov     eax,edi
        sub     eax,base_ofs
        cmp     eax,H_RES*V_RES-(H_RES-H_CHR*8)/2-(V_RES-V_CHR*16)/2*H_RES
        jnbe    @@12
        cmp     byte ptr [cursor_sync],1
        jnz     @@7
        movzx   eax,pos_x
        cmp     al,cur_pos_lo
        jnz     @@7
        mov     ax,cur_shape
        cmp     byte_pos,ah
        jb      @@7
        cmp     byte_pos,al
        ja      @@7
        movzx   eax,pos_y
        cmp     al,cur_pos_hi
        jnz     @@7
        mov     al,[ebx+1]
        and     al,01111b
        stosb
        jmp     @@9
@@7:    movzx   eax,bit_mask
        test    dl,al
        jz      @@8
        mov     al,[ebx+1]
        and     al,01111b
        stosb
        jmp     @@9
@@8:    mov     al,[ebx+1]
        shr     al,4
        stosb
@@9:    dec     loop_idx4
        cmp     loop_idx4,0
        ja      @@5
        add     ebx,2
        inc     pos_x
        dec     loop_idx3
        cmp     loop_idx3,0
        ja      @@4
        sub     ebx,H_CHR*2
        cmp     skip_ofs,0
        jz      @@10
        sub     skip_ofs,H_RES-H_CHR*8
        jmp     @@11
@@10:   add     edi,H_RES-H_CHR*8
@@11:   inc     byte_pos
        dec     loop_idx2
        cmp     loop_idx2,0
        ja      @@3
        inc     pos_y
        add     ebx,H_CHR*2
        dec     loop_idx1
        cmp     loop_idx1,0
        ja      @@2
@@12:
        pop     edi
        pop     esi
        pop     ebx
  end;
end;
