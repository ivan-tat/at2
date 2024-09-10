// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure synchronize_song_timer;
begin
  song_timer := TRUNC(time_playing);
  song_timer_tenths := TRUNC(time_playing*100) MOD 100;
  timer_temp := song_timer_tenths;
end;
