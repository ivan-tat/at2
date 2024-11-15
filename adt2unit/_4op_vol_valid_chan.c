// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
/*static*/ bool _4op_vol_valid_chan (uint8_t chan)
{
  chan--;

  uint32_t _4op_flag = _4op_data_flag (chan + 1);

  return    (_4op_flag & 1)              // 1 bit:  A0
         &&  vol4op_lock[chan]
         && (_4op_flag & (0xFFL << 11))  // 8 bits: E7-E0
         && (_4op_flag & (0xFFL << 19)); // 8 bits: F7-F0
}
