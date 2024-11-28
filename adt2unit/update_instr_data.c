// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// ins: 1...
void update_instr_data (uint8_t ins)
{
  ins--;

  for (uint8_t chan = 0; chan <= 19; chan++)
    if (voice_table[chan] == (ins + 1))
    {
      reset_chan[chan] = true;
      set_ins_data (ins + 1, chan + 1);
      change_frequency (chan + 1, nFreq ((event_table[chan].note & 0x7F) - 1)
                                  + (int8_t)(ins_parameter (ins + 1, 12)));
    }
}
