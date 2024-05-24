// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void eg_key_on_off (OPL3_SLOT *p_slot, bool key_on) {
  if (key_on) {
    if (!p_slot->key) {
      p_slot->eg_state = EG_ATTACK;
      envelope_update_rate (p_slot);
      if ((p_slot->eg_rate >> 2) == 0x0F) {
        p_slot->eg_state = EG_DECAY;
        envelope_update_rate (p_slot);
        p_slot->eg_rout = 0;
      }
      p_slot->pg_phase = 0;
    }

    if (p_slot->p_chan->ch_type == CH_RHYTHM)
      p_slot->key |= 0x02;
    else
      p_slot->key |= 0x01;

    if (!p_slot->reg_ar) {
      // Faked decay prevents restart of envelope if AR==0
      p_slot->eg_state = EG_DECAY;
      p_slot->eg_rout++;
    }
  } else {
    // Key off
    if (p_slot->key) {
      if (p_slot->p_chan->ch_type == CH_RHYTHM)
        p_slot->key &= ~0x02;
      else
        p_slot->key &= ~0x01;

      if (!p_slot->key) {
        p_slot->eg_state = EG_RELEASE;
        envelope_update_rate (p_slot);
      }
    }
  }
}
