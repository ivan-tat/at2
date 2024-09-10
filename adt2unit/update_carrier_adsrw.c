// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
void update_carrier_adsrw (uint8_t chan) {
  opl3out (_instr[5] + _chan_c[chan - 1],
           (fmpar_table[chan - 1].adsrw_car.attck << 4)
           + fmpar_table[chan - 1].adsrw_car.dec);
  opl3out (_instr[7] + _chan_c[chan - 1],
           (fmpar_table[chan - 1].adsrw_car.sustn << 4)
           + fmpar_table[chan - 1].adsrw_car.rel);
  opl3out (_instr[9] + _chan_c[chan - 1],
           fmpar_table[chan - 1].adsrw_car.wform);
}
