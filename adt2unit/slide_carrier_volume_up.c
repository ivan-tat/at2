// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 0..19
static void slide_carrier_volume_up (uint8_t chan, uint8_t slide, uint8_t limit)
{
  uint8_t vLo = volume_table[chan] & 0xFF;
  uint8_t vHi = volume_table[chan] >> 8;

  vHi =  (((int16_t)vHi - slide) >= limit) ? vHi - slide : limit;

  set_ins_volume (BYTE_NULL, vHi, chan + 1);
  volume_table[chan] = vLo + (vHi << 8);
}
