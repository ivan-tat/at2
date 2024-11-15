// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
void key_on (uint8_t chan) {
  chan--;

  if (is_4op_chan (chan + 1) && bit_test (_4op_tracks_hi, chan + 1))
    chan++;

  opl3out (0xB0 + _chan_n[chan], 0);
}
