// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure sys_init;
begin
{$IFNDEF GO32V2}
  vid_Init; // SDL video
  AdT2opl3.snd_init; // SDL sound + opl3 emulation
{$ENDIF}
end;
