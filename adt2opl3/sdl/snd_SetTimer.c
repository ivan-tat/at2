// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// value in Hz for timer
void snd_SetTimer (int32_t value) {
  if (value < PIT_FREQ_FLOOR)
    value = PIT_FREQ_FLOOR;

  sample_frame_size = (int64_t) (sdl_sample_rate / value
                                 * (1 + sdl_timer_slowdown / 100));
}
