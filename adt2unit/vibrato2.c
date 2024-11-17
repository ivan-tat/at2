// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
/*static*/ void vibrato2 (uint8_t chan)
{
  uint16_t freq, old_freq;

  chan--;

  vibr_table2[chan].pos += vibr_table2[chan].speed * vibtrem_speed_factor;
  freq = calc_vibtrem_shift (chan + 1, vibr_table2);
  old_freq = freq_table[chan];
  if (!vibr_table2[chan].dir)
    portamento_down (chan + 1, freq, nFreq (0));
  else
    portamento_up (chan + 1, freq, nFreq (12*8+1));
  freq_table[chan] = old_freq;
}
