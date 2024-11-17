// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
/*static*/ void tremolo (uint8_t chan)
{
  uint16_t vol, old_vol;

  chan--;

  trem_table[chan].pos += trem_table[chan].speed * vibtrem_speed_factor;
  vol = calc_vibtrem_shift (chan + 1, trem_table);
  old_vol = volume_table[chan];
  if (!trem_table[chan].dir)
    slide_volume_down (chan + 1, vol);
  else
    slide_volume_up (chan + 1, vol);
  volume_table[chan] = old_vol;
}
