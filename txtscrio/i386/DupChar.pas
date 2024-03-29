// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure DupChar; assembler;
asm
        pushad                           {  IN/ al     -column        }
        xor     ebx,ebx                  {      ah     -line          }
        xchg    ax,bx                    {      dl     -character     }
        xor     eax,eax                  {      dh     -attribute     }
        xchg    ax,bx                    {      ecx    -count         }
        mov     bl,al                    {      edi    -ptr. to write }
        mov     al,MaxCol
        mul     ah
        add     ax,bx
        mov     bl,MaxCol
        sub     ax,bx
        dec     ax
        shl     ax,1
        jecxz   @@1
        add     edi,eax
        xchg    ax,dx
        rep     stosw
        xchg    ax,dx
@@1:    mov     absolute_pos,ax
        popad
end;
