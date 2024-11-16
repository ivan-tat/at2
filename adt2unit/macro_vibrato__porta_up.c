// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
/*static*/ void macro_vibrato__porta_up (uint8_t chan, uint8_t depth)
{
  chan--;

  uint16_t freq = calc_freq_shift_up (macro_table[chan].vib_freq & 0x1FFF, depth);
  uint16_t limit = nFreq (12*8+1);

  change_freq (chan + 1, (freq <= limit) ? freq : limit);
}
