// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void update_ksl_tl (OPL3_SLOT *p_slot, uint8_t data) {
  p_slot->reg_ksl = (data >> 6) & 0x03;
  p_slot->reg_tl = data & 0x3F;
  envelope_update_ksl (p_slot);
}
