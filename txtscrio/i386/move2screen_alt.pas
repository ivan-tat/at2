// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure move2screen_alt;

var
  pos1,pos2: Byte;

begin
  If (move_to_screen_data <> NIL) then
    asm
        mov     esi,dword ptr [screen_ptr]
        lea     edi,[temp_screen2]
        mov     ecx,SCREEN_MEM_SIZE
        rep     movsb
        mov     esi,[move_to_screen_data]
        mov     edi,dword ptr [ptr_temp_screen2]
        xor     ecx,ecx
        mov     cl,byte ptr [move_to_screen_area+1]
@@1:    mov     pos2,cl
        xor     ecx,ecx
        mov     cl,byte ptr [move_to_screen_area+0]
@@2:    mov     pos1,cl
        mov     al,pos1
        mov     ah,pos2
        xor     ecx,ecx
        call    DupChar
        movzx   ecx,absolute_pos
        mov     ax,word ptr [esi+ecx]
        mov     word ptr [edi+ecx],ax
        movzx   ecx,pos1
        inc     ecx
        cmp     cl,byte ptr [move_to_screen_area+2]
        jbe     @@2
        movzx   ecx,pos2
        inc     ecx
        cmp     cl,byte ptr [move_to_screen_area+3]
        jbe     @@1
        lea     esi,[temp_screen2]
        mov     edi,dword ptr [screen_ptr]
        mov     ecx,SCREEN_MEM_SIZE
        rep     movsb
    end;
end;
