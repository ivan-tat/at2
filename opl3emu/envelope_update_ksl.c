// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void envelope_update_ksl (OPL3_SLOT *p_slot) {
  int16_t ksl = (KSL_VAL[p_slot->p_chan->fnum >> 6] << 2) -
                ((8 - p_slot->p_chan->block) << 5);

  p_slot->eg_ksl = limit_value (ksl, 0, 255);
}
