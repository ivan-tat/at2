// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
/*static*/ void portamento_down (uint8_t chan, uint16_t slide, uint16_t limit)
{
  chan--;

  if (freq_table[chan] & 0x1FFF)
  {
    uint16_t freq = calc_freq_shift_down (freq_table[chan] & 0x1FFF, slide);

    change_frequency (chan + 1, (freq >= limit) ? freq : limit);
  }
}
