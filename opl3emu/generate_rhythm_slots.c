// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void calc_phase_slot7_slot8 (const OPL3_CHIP *p_chip,
                                    uint16_t *phase_lo, uint16_t *phase_hi,
                                    uint16_t *phase_bit) {

  *phase_lo = (p_chip->chan[7].p_slot[0]->pg_phase >> 9) & 0x3FF;
  *phase_hi = (p_chip->chan[8].p_slot[1]->pg_phase >> 9) & 0x3FF;
  *phase_bit = ((*phase_lo & 8)
                || (((*phase_lo >> 5) ^ *phase_lo) & 4)
                || (((*phase_hi >> 2) ^ *phase_hi) & 8)) ? 1 : 0;
}

static void generate_rhythm_slots (OPL3_CHIP *p_chip) {
  OPL3_SLOT *p_slot;
  uint16_t phase, phase_lo, phase_hi, phase_bit;

  // BD
  p_slot = p_chip->chan[6].p_slot[0];
  slot_generate (p_slot, (uint16_t)(p_slot->pg_phase >> 9) + *p_slot->p_mod);
  p_slot = p_chip->chan[6].p_slot[1];
  slot_generate (p_slot, (uint16_t)(p_slot->pg_phase >> 9) + *p_slot->p_mod);

  // HH
  p_slot = p_chip->chan[7].p_slot[0];
  calc_phase_slot7_slot8 (p_chip, &phase_lo, &phase_hi, &phase_bit);
  phase = (phase_bit << 9)
          | (uint16_t)(0x34 << ((phase_bit ^ (p_chip->noise & 1) << 1)));
  slot_generate (p_slot, phase);

  // TT
  p_slot = p_chip->chan[8].p_slot[0];
  slot_generate (p_slot, (uint16_t)(p_slot->pg_phase >> 9));

  // SD
  p_slot = p_chip->chan[7].p_slot[1];
  calc_phase_slot7_slot8 (p_chip, &phase_lo, &phase_hi, &phase_bit);
  phase = (0x100 << ((phase_lo >> 8) & 1))
          ^ (uint16_t)((p_chip->noise & 1) << 8);
  slot_generate (p_slot, phase);

  // TC
  p_slot = p_chip->chan[8].p_slot[1];
  phase = 0x100 | (phase_bit << 9);
  slot_generate (p_slot, phase);
}
