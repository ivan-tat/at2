// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
void update_modulator_adsrw (uint8_t chan) {
  opl3out (_instr[4] + _chan_m[chan - 1],
           (fmpar_table[chan - 1].adsrw_mod.attck << 4)
           + fmpar_table[chan - 1].adsrw_mod.dec);
  opl3out (_instr[6] + _chan_m[chan - 1],
           (fmpar_table[chan - 1].adsrw_mod.sustn << 4)
           + fmpar_table[chan - 1].adsrw_mod.rel);
  opl3out (_instr[8] + _chan_m[chan - 1],
           fmpar_table[chan - 1].adsrw_mod.wform);
}
