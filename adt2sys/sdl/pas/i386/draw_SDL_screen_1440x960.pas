// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure draw_SDL_screen_1440x960;

var
   byte_pos,bit_mask: Byte;
   pos_x,pos_y: Byte;
   loop_idx1,loop_idx2,loop_idx3,
   loop_idx4: Dword;

begin
  asm
        mov     eax,_cursor_blink_factor
        cmp     _cursor_blink_pending_frames,eax
        jnae    @@1
        mov     _cursor_blink_pending_frames,0
        xor     byte ptr [cursor_sync],1
@@1:    lea     esi,[font8x16]
        mov     edi,dword ptr [_FrameBuffer]
        mov     ebx,dword ptr [screen_ptr]
        mov     loop_idx1,60
        mov     pos_y,1
@@2:    mov     byte_pos,0
        mov     loop_idx2,16
@@3:    mov     loop_idx3,180
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
@@6:    cmp     cursor_sync,1
        jnz     @@7
        movzx   eax,byte ptr [pos_x]
        cmp     al,byte ptr [virtual_cur_pos]
        jnz     @@7
        mov     ax,word ptr [virtual_cur_shape]
        cmp     byte_pos,ah
        jb      @@7
        cmp     byte_pos,al
        ja      @@7
        movzx   eax,pos_y
        cmp     al,byte ptr [virtual_cur_pos+1]
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
        sub     ebx,180*2
        inc     byte_pos
        dec     loop_idx2
        cmp     loop_idx2,0
        ja      @@3
        inc     pos_y
        add     ebx,180*2
        dec     loop_idx1
        cmp     loop_idx1,0
        ja      @@2
  end;
end;
