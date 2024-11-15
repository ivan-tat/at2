// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
/*static*/ void reset_ins_volume (uint8_t chan)
{
  chan--;

  if (!volume_scaling)
    set_ins_volume (ins_parameter (voice_table[chan], 2) & 0x3F,
                    ins_parameter (voice_table[chan], 3) & 0x3F, chan + 1);
  else
    if (!(ins_parameter (voice_table[chan], 10) & 1))
      set_ins_volume (ins_parameter (voice_table[chan], 2) & 0x3F, 0, chan + 1);
    else
      set_ins_volume (0, 0, chan + 1);
}
