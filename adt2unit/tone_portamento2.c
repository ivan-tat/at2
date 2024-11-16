// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
/*static*/ void tone_portamento2 (uint8_t chan)
{
  chan--;

  if ((freq_table[chan] & 0x1FFF) > porta_table2[chan].freq)
    portamento_down (chan + 1, porta_table2[chan].speed, porta_table2[chan].freq);
  else
    if ((freq_table[chan] & 0x1FFF) < porta_table2[chan].freq)
      portamento_up (chan + 1, porta_table2[chan].speed, porta_table2[chan].freq);
}
