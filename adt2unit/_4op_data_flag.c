// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
// Return value:
//   +------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//   | BIT  |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
//   +------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//   | DATA |      0       |F7 F6 F5 F4 F3 F2 F1 F0|E7 E6 E5 E4 E3 E2 E1 E0|D3 D2 D1 D0|C3 C2 C1 C0|B1 B0|A0|
//   +------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//     A: 1 bit  (A0)
//     B: 2 bits (B1-B0)
//     C: 4 bits (C3-C0)
//     D: 4 bits (D3-D0)
//     E: 8 bits (E7-E0)
//     F: 8 bits (F7-F0)
/*static*/ uint32_t _4op_data_flag (uint8_t chan)
{
  bool _4op_mode = false;
  uint8_t _4op_conn = 0;
  int8_t _4op_ch1 = -1, _4op_ch2 = -1;
  int_least16_t _4op_ins1 = -1, _4op_ins2 = -1;

  chan--;

  if (is_4op_chan (chan + 1))
  {
    _4op_mode = true;

    if (bit_test (_4op_tracks_hi, chan + 1))
    {
      _4op_ch1 = chan;
      _4op_ch2 = chan + 1;
    }
    else
    {
      _4op_ch1 = chan - 1;
      _4op_ch2 = chan;
    }

    _4op_ins1 = event_table[_4op_ch1].instr_def - 1;
    if (_4op_ins1 < 0)
      _4op_ins1 = voice_table[_4op_ch1] - 1;

    _4op_ins2 = event_table[_4op_ch2].instr_def - 1;
    if (_4op_ins2 < 0)
      _4op_ins2 = voice_table[_4op_ch2] - 1;

    if ((_4op_ins1 >= 0) && (_4op_ins2 >= 0))
    {
      uint8_t i = _instr_data_ofs[10];

      _4op_conn = ((((((uint8_t *)&songdata.instr_data[_4op_ins1])[i]) & 1) << 1)
                +   ((((uint8_t *)&songdata.instr_data[_4op_ins2])[i]) & 1)) & 3;
    }
  }

  return   (_4op_mode ? 1 : 0)            // A
         + (_4op_conn             <<  1)  // B
         + (((_4op_ch1 + 1) & 15) <<  3)  // C
         + (((_4op_ch2 + 1) & 15) <<  7)  // D
         + ((_4op_ins1 + 1)       << 11)  // E
         + ((_4op_ins2 + 1)       << 19); // F
}
