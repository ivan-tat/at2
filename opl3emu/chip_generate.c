// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static uint32_t chip_generate (OPL3_CHIP *p_chip) {
  uint8_t i; // slot number
  int16_t lr_mix[2];

  // Generate slot data
  for (i = 0; i < 36; i++) {
    slot_calc_fb (&p_chip->slot[i]);
    pg_generate (&p_chip->slot[i]);
    envelope_calc (&p_chip->slot[i]);
    slot_generate (&p_chip->slot[i], WORD_NULL);
  }

  // Rhythm mode
  if (p_chip->rhy_flag & 0x20)
    generate_rhythm_slots (p_chip);

  // Update channel mixer
  lr_mix[0] = limit_value (p_chip->output[0], -0x7FFF, 0x7FFF);
  lr_mix[1] = limit_value (p_chip->output[1], -0x7FFF, 0x7FFF);
  p_chip->output[0] = 0;
  p_chip->output[1] = 0;

  // Left output
  for (i = 0; i < 18; i++) {
    int16_t accm = *p_chip->chan[i].p_out[0]
                   + *p_chip->chan[i].p_out[1]
                   + *p_chip->chan[i].p_out[2]
                   + *p_chip->chan[i].p_out[3];
    p_chip->output[0] += (int16_t)(accm & p_chip->chan[i].out_l);
    p_chip->out_l[i] = (int16_t)(accm & p_chip->chan[i].out_l);
  }

  // Right output
  for (i = 0; i < 18; i++) {
    int16_t accm = *p_chip->chan[i].p_out[0]
                   + *p_chip->chan[i].p_out[1]
                   + *p_chip->chan[i].p_out[2]
                   + *p_chip->chan[i].p_out[3];
    p_chip->output[1] += (int16_t)(accm & p_chip->chan[i].out_r);
    p_chip->out_r[i] = (int16_t)(accm & p_chip->chan[i].out_r);
  }

  // Update LFO tremolo
  if ((p_chip->timer & 0x3F) == 0x3F) {
    if (!p_chip->trem_dir) {
      if (p_chip->trem_pos < 105)
        p_chip->trem_pos++;
      else {
        p_chip->trem_pos--;
        p_chip->trem_dir = 1;
      }
    } else {
      if (p_chip->trem_pos > 0)
        p_chip->trem_pos--;
      else {
        p_chip->trem_pos++;
        p_chip->trem_dir = 0;
      }
    }

    p_chip->trem_val = (p_chip->trem_pos >> 2)
                       >> ((1 - p_chip->dva_bit) << 1);
  }

  // Update noise generator
  if (p_chip->noise & 1)
    p_chip->noise ^= NOISE_XOR;

  p_chip->noise >>= 1;
  p_chip->timer++;

  return ((uint16_t)lr_mix[1] << 16) | (uint16_t)lr_mix[0];
}
