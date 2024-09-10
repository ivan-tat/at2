// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
void key_on (uint8_t chan) {
  if (is_4op_chan (chan) && bit_test (_4op_tracks_hi, chan))
    opl3out (0xB0 + _chan_n[chan - 1 + 1], 0);
  else
    opl3out (0xB0 + _chan_n[chan - 1], 0);
}
