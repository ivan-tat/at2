// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void chan_update_4op (OPL3_CHIP *p_chip, uint8_t data) {
  uint8_t i; // Bit number

  for (i = 0; i <= 5; i++) {
    uint8_t j = CH_4OP_IDX[i];

    if (data & 1) {
      // Set chan. to 4OP
      p_chip->chan[j].ch_type = CH_4OP_1;
      p_chip->chan[j + 3].ch_type = CH_4OP_2;
    } else {
      // Reset chan. to 2OP
      p_chip->chan[j].ch_type = CH_2OP;
      p_chip->chan[j + 3].ch_type = CH_2OP;
    }

    data >>= 1;
  }
}
