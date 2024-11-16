// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 0..19
static void slide_modulator_volume_down (uint8_t chan, uint8_t slide)
{
  uint8_t vLo = volume_table[chan] & 0xFF;
  uint8_t vHi = volume_table[chan] >> 8;

  vLo = ((vLo + slide) <= 63) ? vLo + slide : 63;

  set_ins_volume (vLo, BYTE_NULL, chan + 1);
  volume_table[chan] = vLo + (vHi << 8);
}
