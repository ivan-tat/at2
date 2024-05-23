// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure newint09_proc; assembler;
asm
        cli
        push    ds
        push    es
        push    fs
        push    gs
        pushad
        mov     ax,cs:[backupDS_adt2keyb]
        mov     ds,ax
        mov     es,ax
        mov     ax,DosMemSelector
        mov     fs,ax
        call    dword ptr [user_proc_ptr]
        popad
        pop     gs
        pop     fs
        pop     es
        pop     ds
        jmp     cs:[oldint09_handler]
end;

procedure newint09_proc_end; begin end;
