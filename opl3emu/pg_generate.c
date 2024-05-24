// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void pg_generate (OPL3_SLOT *p_slot) {
  uint16_t fnum = p_slot->p_chan->fnum;

  if (p_slot->reg_vib) {
    // Apply LFO vibrato
    uint8_t fnum_hi = fnum >> (7 + VIB_SHIFT[(p_slot->p_chip->timer >> 10) & 7]
                               + (1 - p_slot->p_chip->dvb_bit));

    fnum += fnum_hi * VIB_S_SHIFT[(p_slot->p_chip->timer >> 10) & 7];
  }

  p_slot->pg_phase += (((fnum << p_slot->p_chan->block) >> 1)
                       * MULT_VAL[p_slot->reg_mult]) >> 1;
}
