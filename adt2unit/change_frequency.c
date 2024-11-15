// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
void change_frequency (uint8_t chan, uint16_t freq) {
  chan--;

  macro_table[chan].vib_paused = true;
  change_freq (chan + 1, freq);

  if (is_4op_chan (chan + 1)) {
    uint8_t i = bit_test (_4op_tracks_hi, chan + 1) ? chan + 1 : chan - 1;

    macro_table[i].vib_count = 1;
    macro_table[i].vib_pos = 0;
    macro_table[i].vib_freq = freq;
    macro_table[i].vib_paused = false;
  }

  macro_table[chan].vib_count = 1;
  macro_table[chan].vib_pos = 0;
  macro_table[chan].vib_freq = freq;
  macro_table[chan].vib_paused = false;
}
