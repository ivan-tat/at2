// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// value in Hz for timer
procedure snd_SetTimer(value: Longint);
begin
  If (value < 18) then value := 18;
  sample_frame_size := ROUND(sdl_sample_rate/value*(1+sdl_timer_slowdown/100));
end;
