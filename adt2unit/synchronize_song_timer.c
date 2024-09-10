// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*static*/ void synchronize_song_timer (void) {
  song_timer = (int64_t) trunc (time_playing);
  song_timer_tenths = (int64_t) trunc (time_playing * 100) % 100;
  timer_temp = song_timer_tenths;
}
