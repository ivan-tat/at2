// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static uint8_t envelope_calc_rate (const OPL3_SLOT *p_slot,
                                   uint8_t reg_rate) {

  if (reg_rate) {
    uint8_t rate = (reg_rate << 2)
                 + (p_slot->reg_ksr ? p_slot->p_chan->ksr
                                    : (p_slot->p_chan->ksr >> 2));
    return limit_value (rate, 0, 60);
  } else
    return 0;
}
