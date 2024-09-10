// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
void release_sustaining_sound (uint8_t chan) {
  opl3out (_instr[2] + _chan_m[chan - 1], 63);
  opl3out (_instr[3] + _chan_c[chan - 1], 63);

  memset (&fmpar_table[chan - 1].adsrw_car, 0,
          sizeof(fmpar_table[chan - 1].adsrw_car));
  memset (&fmpar_table[chan - 1].adsrw_mod, 0,
          sizeof(fmpar_table[chan - 1].adsrw_mod));

  key_on (chan);
  opl3out (_instr[4] + _chan_m[chan - 1], BYTE_NULL);
  opl3out (_instr[5] + _chan_c[chan - 1], BYTE_NULL);
  opl3out (_instr[6] + _chan_m[chan - 1], BYTE_NULL);
  opl3out (_instr[7] + _chan_c[chan - 1], BYTE_NULL);

  key_off (chan);
  event_table[chan - 1].instr_def = 0;
  reset_chan[chan - 1] = true;
}
