// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void slot_generate (OPL3_SLOT *p_slot, uint16_t phase) {
  if (phase == WORD_NULL)
    phase = (uint16_t)(p_slot->pg_phase >> 9) + *p_slot->p_mod;

  p_slot->output = envelope_calc_sin (p_slot->reg_wf, phase, p_slot->eg_out);
}
