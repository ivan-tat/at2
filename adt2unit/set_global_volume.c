// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void set_global_volume (void)
{
  for (uint8_t chan = 0; chan < songdata.nm_tracks; chan++)
    if (_4op_vol_valid_chan (chan + 1))
      set_ins_volume_4op (BYTE_NULL, chan + 1);
    else
      if (carrier_vol[chan] || modulator_vol[chan])
        set_ins_volume ((ins_parameter (voice_table[chan], 10) & 1) ? volume_table[chan] & 0xFF
                                                                    : BYTE_NULL,
                        volume_table[chan] >> 8, chan + 1);
}
