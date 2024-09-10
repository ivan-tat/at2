// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
void change_freq (uint8_t chan, uint16_t freq) {
  uint16_t x;

  if (is_4op_chan (chan) && bit_test (_4op_tracks_hi, chan)) {
    freq_table[chan - 1 + 1] = freq_table[chan - 1];
    freqtable2[chan - 1 + 1] = freqtable2[chan - 1];
    chan++;
  }

  x = (freq & 0x1FFF) + (freq_table[chan - 1] & ~0x1FFF);
  freq_table[chan - 1] = x;
  freqtable2[chan - 1] = x;

  if (channel_flag[chan - 1]) {
    opl3out (_chan_n[chan - 1] + 0xA0, x & 0xFF);
    opl3out (_chan_n[chan - 1] + 0xB0, x >> 8);
  }

  if (is_4op_chan (chan)) {
    freq_table[chan - 1 - 1] = freq_table[chan - 1];
    freqtable2[chan - 1 - 1] = freqtable2[chan - 1];
  }
}
