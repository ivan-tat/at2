// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Uses `arpgg_state[]' from `arpeggio()'

// chan: 1..20
/*static*/ void arpeggio2 (uint8_t chan)
{
  uint16_t freq;

  chan--;

  switch (arpgg_table2[chan].state)
  {
    case 0:
      freq = nFreq (arpgg_table2[chan].note - 1);
      break;

    case 1:
      freq = nFreq (arpgg_table2[chan].note - 1 + arpgg_table2[chan].add1);
      break;

    case 2:
      freq = nFreq (arpgg_table2[chan].note - 1 + arpgg_table2[chan].add2);
      break;

    default: // FIXME: never reached?
      freq = 0; // HINT: added when ported to C
      break;
  }

  arpgg_table2[chan].state = arpgg_state[arpgg_table2[chan].state];
  change_frequency (chan + 1, freq + (int8_t)ins_parameter (event_table[chan].instr_def, 12));
}
