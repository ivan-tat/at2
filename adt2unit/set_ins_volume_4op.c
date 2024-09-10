// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
/*static*/ void set_ins_volume_4op (uint8_t volume, uint8_t chan) {
  uint32_t _4op_flag = _4op_data_flag (chan);
  uint8_t _4op_conn = (_4op_flag >> 1) & 3;
  uint8_t _4op_ch1 = (_4op_flag >> 3) & 15;
  uint8_t _4op_ch2 = (_4op_flag >> 7) & 15;

  if (_4op_vol_valid_chan (chan))
    switch (_4op_conn) {
    case 0: // FM/FM
      if (volume == BYTE_NULL)
        set_volume (BYTE_NULL, (volume_table[_4op_ch1 - 1] >> 8) & 0xFF, _4op_ch1);
      else
        set_volume (BYTE_NULL, volume,_4op_ch1);
      break;

    case 1: // FM/AM
      if (volume == BYTE_NULL) {
        set_volume (BYTE_NULL, (volume_table[_4op_ch1 - 1] >> 8) & 0xFF, _4op_ch1);
        set_volume (volume_table[_4op_ch2 - 1] & 0xFF, BYTE_NULL, _4op_ch2);
      } else  {
        set_volume (BYTE_NULL, volume, _4op_ch1);
        set_volume (volume, BYTE_NULL, _4op_ch2);
      }
      break;

    case 2: // AM/FM
      if (volume == BYTE_NULL) {
        set_volume (BYTE_NULL, (volume_table[_4op_ch1 - 1] >> 8) & 0xFF, _4op_ch1);
        set_volume (BYTE_NULL, (volume_table[_4op_ch2 - 1] >> 8) & 0xFF, _4op_ch2);
      } else {
        set_volume (BYTE_NULL, volume, _4op_ch1);
        set_volume (BYTE_NULL, volume, _4op_ch2);
      }
      break;

    case 3: // AM/AM
      if (volume == BYTE_NULL) {
        set_volume (volume_table[_4op_ch1 - 1] & 0xFF, (volume_table[_4op_ch1 - 1] >> 8) & 0xFF, _4op_ch1);
        set_volume (volume_table[_4op_ch2 - 1] & 0xFF, BYTE_NULL, _4op_ch2);
      } else {
        set_volume (volume, volume, _4op_ch1);
        set_volume (volume, BYTE_NULL, _4op_ch2);
      }
      break;

    default:
      break;
  }
}
