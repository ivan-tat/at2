// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
void update_fmpar (uint8_t chan)
{
  chan--;

  opl3out (_instr[ 0] + _chan_m[chan],    fmpar_table[chan].multipM
                                       + (fmpar_table[chan].ksrM  << 4)
                                       + (fmpar_table[chan].sustM << 5)
                                       + (fmpar_table[chan].vibrM << 6)
                                       + (fmpar_table[chan].tremM << 7));
  opl3out (_instr[ 1] + _chan_c[chan],    fmpar_table[chan].multipC
                                       + (fmpar_table[chan].ksrC  << 4)
                                       + (fmpar_table[chan].sustC << 5)
                                       + (fmpar_table[chan].vibrC << 6)
                                       + (fmpar_table[chan].tremC << 7));

  opl3out (_instr[10] + _chan_n[chan], (   fmpar_table[chan].connect
                                        + (fmpar_table[chan].feedb << 1))
                                       | _panning[panning_table[chan]]);

  vscale_table[chan] =   ((fmpar_table[chan].kslM & 3) << 6)
                       + ((fmpar_table[chan].kslC & 3) << 14);
  set_ins_volume (volume_table[chan] & 0xFF,
                  volume_table[chan] >> 8, chan + 1);
}
