// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure ScreenMemCopy(source,dest: tSCREEN_MEM_PTR);
begin
  cursor_backup := GetCursor;
  asm
        xor     edx,edx
        mov     eax,SCREEN_MEM_SIZE
        cmp     eax,16
        jb      @@1
        mov     ecx,4
        div     ecx
        mov     ecx,eax
        jecxz   @@1
        mov     esi,dword ptr [source]
        mov     edi,dword ptr [dest]
        cld
        rep     movsd
        mov     ecx,edx
        jecxz   @@2
        rep     movsb
        jmp     @@2
@@1:    mov     ecx,SCREEN_MEM_SIZE
        mov     esi,dword ptr [source]
        mov     edi,dword ptr [dest]
        cld
        rep     movsb
@@2:
  end;
end;
