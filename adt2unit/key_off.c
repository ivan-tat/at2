// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
void key_off (uint8_t chan) {
  freq_table[chan - 1] &= ~0x2000;
  change_freq (chan, freq_table[chan - 1]);
  event_table[chan - 1].note |= keyoff_flag;
}
