// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function is_chan_adsr_data_empty(chan: Byte): Boolean;
begin
  is_chan_adsr_data_empty :=
    (fmpar_table[chan].adsrw_car.attck = 0) and
    (fmpar_table[chan].adsrw_mod.attck = 0) and
    (fmpar_table[chan].adsrw_car.dec = 0) and
    (fmpar_table[chan].adsrw_mod.dec = 0) and
    (fmpar_table[chan].adsrw_car.sustn = 0) and
    (fmpar_table[chan].adsrw_mod.sustn = 0) and
    (fmpar_table[chan].adsrw_car.rel = 0) and
    (fmpar_table[chan].adsrw_mod.rel = 0);
end;
