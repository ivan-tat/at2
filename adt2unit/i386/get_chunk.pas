// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure get_chunk(pattern,line,channel: Byte; var chunk: tCHUNK);
begin
  asm
        mov     esi,[pattdata]
        mov     edi,[chunk]
        mov     al,pattern
        inc     al
        cmp     al,max_patterns
        jbe     @@1
        mov     ecx,CHUNK_SIZE
        xor     al,al
        rep     stosb
        jmp     @@2
@@1:    xor     eax,eax
        mov     al,line
        mov     ebx,CHUNK_SIZE
        mul     ebx
        mov     ecx,eax
        xor     eax,eax
        mov     al,channel
        dec     eax
        mov     ebx,256*CHUNK_SIZE
        mul     ebx
        add     ecx,eax
        xor     eax,eax
        mov     al,pattern
        mov     ebx,8
        div     ebx
        push    eax
        mov     eax,edx
        mov     ebx,20*256*CHUNK_SIZE
        mul     ebx
        add     ecx,eax
        pop     eax
        mov     ebx,8*20*256*CHUNK_SIZE
        mul     ebx
        add     ecx,eax
        add     esi,ecx
        mov     ecx,CHUNK_SIZE
        rep     movsb
@@2:
  end;
end;
