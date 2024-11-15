// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
bool is_4op_chan (uint8_t chan) {
  chan--;

  return (        (songdata.flag_4op & 0x3F)
          && (   ((songdata.flag_4op & (1 << 0)) && ((chan ==  0) || (chan ==  1)))
              || ((songdata.flag_4op & (1 << 1)) && ((chan ==  2) || (chan ==  3)))
              || ((songdata.flag_4op & (1 << 2)) && ((chan ==  4) || (chan ==  5)))
              || ((songdata.flag_4op & (1 << 3)) && ((chan ==  9) || (chan == 10)))
              || ((songdata.flag_4op & (1 << 4)) && ((chan == 11) || (chan == 12)))
              || ((songdata.flag_4op & (1 << 5)) && ((chan == 13) || (chan == 14)))));
}
