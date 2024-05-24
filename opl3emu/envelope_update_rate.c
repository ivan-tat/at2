// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void envelope_update_rate (OPL3_SLOT *p_slot) {
  switch (p_slot->eg_state) {
  case EG_OFF:
    p_slot->eg_rate = 0;
    break;

  case EG_ATTACK:
    p_slot->eg_rate = envelope_calc_rate (p_slot, p_slot->reg_ar);
    break;

  case EG_DECAY:
    p_slot->eg_rate = envelope_calc_rate (p_slot, p_slot->reg_dr);
    break;

  case EG_SUSTAIN:
  case EG_RELEASE:
    p_slot->eg_rate = envelope_calc_rate (p_slot, p_slot->reg_rr);
    break;

  default:
    break;
  }
}
