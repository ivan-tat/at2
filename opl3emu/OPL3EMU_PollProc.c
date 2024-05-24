// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void OPL3EMU_PollProc (uint32_t *p_data, void *ch_table) {
  uint8_t i; // Channel number

  // Assign main output
  *p_data = chip_generate (&opl3);

  // Assign per-channel output
  for (i = 0; i < 18; i++) {
    uint8_t j = CH_MAPPING[i];

    *(*((CHAN_PTR_TABLE *)ch_table))[i] = ((uint16_t)opl3.out_r[j] << 16)
                                          | (uint16_t)opl3.out_l[j];
  }

  // Sort rhythm channels as last
  if (opl3.rhy_flag & 0x20) {
    uint32_t t;

    t = *(*((CHAN_PTR_TABLE *)ch_table))[15];
    *(*((CHAN_PTR_TABLE *)ch_table))[15] = *(*((CHAN_PTR_TABLE *)ch_table))[6];
    *(*((CHAN_PTR_TABLE *)ch_table))[6] = t;

    t = *(*((CHAN_PTR_TABLE *)ch_table))[16];
    *(*((CHAN_PTR_TABLE *)ch_table))[16] = *(*((CHAN_PTR_TABLE *)ch_table))[7];
    *(*((CHAN_PTR_TABLE *)ch_table))[7] = t;

    t = *(*((CHAN_PTR_TABLE *)ch_table))[17];
    *(*((CHAN_PTR_TABLE *)ch_table))[17] = *(*((CHAN_PTR_TABLE *)ch_table))[8];
    *(*((CHAN_PTR_TABLE *)ch_table))[8] = t;
  }
}
