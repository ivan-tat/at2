// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
void change_frequency (uint8_t chan, uint16_t freq) {
  macro_table[chan - 1].vib_paused = true;
  change_freq (chan, freq);

  if (is_4op_chan (chan)) {
    if (bit_test (_4op_tracks_hi, chan)) {
        macro_table[chan - 1 + 1].vib_count = 1;
        macro_table[chan - 1 + 1].vib_pos = 0;
        macro_table[chan - 1 + 1].vib_freq = freq;
        macro_table[chan - 1 + 1].vib_paused = false;
    } else {
        macro_table[chan - 1 - 1].vib_count = 1;
        macro_table[chan - 1 - 1].vib_pos = 0;
        macro_table[chan - 1 - 1].vib_freq = freq;
        macro_table[chan - 1 - 1].vib_paused = false;
    }
  }

  macro_table[chan - 1].vib_count = 1;
  macro_table[chan - 1].vib_pos = 0;
  macro_table[chan - 1].vib_freq = freq;
  macro_table[chan - 1].vib_paused = false;
}
