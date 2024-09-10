// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
bool is_4op_chan (uint8_t chan) {
  if ((songdata.flag_4op & (1 << 0)) && ((chan == 1) || (chan == 2)))
    return true;

  if ((songdata.flag_4op & (1 << 1)) && ((chan == 3) || (chan == 4)))
    return true;

  if ((songdata.flag_4op & (1 << 2)) && ((chan == 5) || (chan == 6)))
    return true;

  if ((songdata.flag_4op & (1 << 3)) && ((chan == 10) || (chan == 11)))
    return true;

  if ((songdata.flag_4op & (1 << 4)) && ((chan == 12) || (chan == 13)))
    return true;

  if ((songdata.flag_4op & (1 << 5)) && ((chan == 14) || (chan == 15)))
    return true;

  return false;
}
