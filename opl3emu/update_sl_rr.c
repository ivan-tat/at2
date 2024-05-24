// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void update_sl_rr (OPL3_SLOT *p_slot, uint8_t data) {
  p_slot->reg_sl = SL_VAL[(data >> 4) & 0x0F];
  p_slot->reg_rr = data & 0x0F;
  envelope_update_rate (p_slot);
}
