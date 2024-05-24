// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void update_lfo_eg_ksr_mult (OPL3_SLOT *p_slot, uint8_t data) {
  // Assign LFO tremolo
  if ((data >> 7) & 1)
    p_slot->p_trem = &p_slot->p_chip->trem_val;
  else
    p_slot->p_trem = (uint8_t *)&p_slot->p_chip->out_null;

  p_slot->reg_vib = (data >> 6) & 1;
  p_slot->reg_type = (data >> 5) & 1;
  p_slot->reg_ksr = (data >> 4) & 1;
  p_slot->reg_mult = data & 0x0F;
  envelope_update_rate (p_slot);
}
