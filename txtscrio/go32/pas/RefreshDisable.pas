// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure RefreshDisable;
begin
  asm
        mov     ax,1201h
        mov     bl,36h
        int     10h
  end;
end;
