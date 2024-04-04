// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure WaitRetrace;
begin
  asm
        mov     dx,3dah
@@1:    in      al,dx
        and     al,08h
        jnz     @@1
@@2:    in      al,dx
        and     al,08h
        jz      @@2
  end;
end;
