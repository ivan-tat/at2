// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

double calc_bpm_speed (uint8_t tempo, uint8_t speed, uint8_t rows_per_beat)
{
  return (double) tempo * 60 / speed / rows_per_beat
#if GO32
         * (1 + (IRQ_freq_shift / IRQ_freq) - 0.02);
#else // !GO32
         * (1 + (IRQ_freq_shift / IRQ_freq) - sdl_timer_slowdown / 100);
#endif // !GO32
}
