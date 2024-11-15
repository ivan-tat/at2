// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
void change_freq (uint8_t chan, uint16_t freq) {
  uint16_t x;

  chan--;

  if (is_4op_chan (chan + 1) && bit_test (_4op_tracks_hi, chan + 1)) {
    freq_table[chan + 1] = freq_table[chan];
#if !ADT2PLAY
    freqtable2[chan + 1] = freqtable2[chan];
#endif // !ADT2PLAY
    chan++;
  }

  x = (freq & 0x1FFF) + (freq_table[chan] & ~0x1FFF);
  freq_table[chan] = x;
#if !ADT2PLAY
  freqtable2[chan] = x;

  if (channel_flag[chan]) {
#endif // !ADT2PLAY
    opl3out (_chan_n[chan] + 0xA0, x & 0xFF);
    opl3out (_chan_n[chan] + 0xB0, x >> 8);
#if !ADT2PLAY
  }
#endif // !ADT2PLAY

  if (is_4op_chan (chan + 1)) {
    freq_table[chan - 1] = freq_table[chan];
#if !ADT2PLAY
    freqtable2[chan - 1] = freqtable2[chan];
#endif // !ADT2PLAY
  }
}
