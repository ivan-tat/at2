// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void envelope_calc (OPL3_SLOT *p_slot) {
  uint8_t rate_hi = p_slot->eg_rate >> 2;
  uint8_t rate_lo = p_slot->eg_rate & 3;

  // Calculate increment step for output
  if (EG_SHIFT[rate_hi] > 0) {
    if (!(p_slot->p_chip->timer & ((1 << EG_SHIFT[rate_hi]) - 1)))
      p_slot->eg_inc = EG_VAL[EG_IDX[rate_hi]][rate_lo]
                             [(p_slot->p_chip->timer >> EG_SHIFT[rate_hi]) & 7];
    else
      p_slot->eg_inc = 0;
  } else
    p_slot->eg_inc = EG_VAL[EG_IDX[rate_hi]][rate_lo]
                           [p_slot->p_chip->timer & 7] << abs (EG_SHIFT[rate_hi]);

  p_slot->eg_out = p_slot->eg_rout
                 + (p_slot->reg_tl << 2)
                 + (p_slot->eg_ksl >> KSL_SHIFT[p_slot->reg_ksl])
                 + *p_slot->p_trem; // Apply LFO tremolo

  switch (p_slot->eg_state) {
  case EG_OFF:
    p_slot->eg_rout = 0x1FF;
    break;

  case EG_ATTACK:
    if (p_slot->eg_rout) {
      p_slot->eg_rout += ((~p_slot->eg_rout) * p_slot->eg_inc) >> 3;
      limit_value (p_slot->eg_rout, 0, 0x1FF);
    } else {
      // Continue with decay if max. level is reached
      p_slot->eg_state = EG_DECAY;
      envelope_update_rate (p_slot);
    }
    break;

  case EG_DECAY:
    if (p_slot->eg_rout < (((int16_t)p_slot->reg_sl) << 4))
      p_slot->eg_rout += p_slot->eg_inc;
    else {
      // Sustain level was reached
      p_slot->eg_state = EG_SUSTAIN;
      envelope_update_rate (p_slot);
    }
    break;

  case EG_SUSTAIN:
  case EG_RELEASE:
    if ((p_slot->eg_state == EG_SUSTAIN) && p_slot->reg_type) {
      // Sustain phase
    } else if (p_slot->eg_rout < 0x1FF)
      p_slot->eg_rout += p_slot->eg_inc;
    else {
      // Switch off generator if min. level is reached
      p_slot->eg_state = EG_OFF;
      p_slot->eg_rout = 0x1FF;
      envelope_update_rate (p_slot);
    }
    break;

  default:
    break;
  }
}
