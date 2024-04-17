// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure ISS_DisableTimerIRQ;
begin
  asm
        in      al,21h
        or      al,1
        out     21h,al
  end;
end;
