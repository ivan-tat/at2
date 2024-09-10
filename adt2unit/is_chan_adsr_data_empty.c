// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
bool is_chan_adsr_data_empty (uint8_t chan) {
  return (fmpar_table[chan - 1].adsrw_car.attck == 0)
         && (fmpar_table[chan - 1].adsrw_mod.attck == 0)
         && (fmpar_table[chan - 1].adsrw_car.dec == 0)
         && (fmpar_table[chan - 1].adsrw_mod.dec == 0)
         && (fmpar_table[chan - 1].adsrw_car.sustn == 0)
         && (fmpar_table[chan - 1].adsrw_mod.sustn == 0)
         && (fmpar_table[chan - 1].adsrw_car.rel == 0)
         && (fmpar_table[chan - 1].adsrw_mod.rel == 0);
}
