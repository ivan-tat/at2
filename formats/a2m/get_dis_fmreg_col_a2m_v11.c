// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_dis_fmreg_col_a2m_v11 (tDIS_FMREG_COL *dst, dis_fmreg_col_a2m_v11_t *src)
{
  (*dst)[ 0] = src->modulator.attack_rate       != 0;
  (*dst)[ 1] = src->modulator.decay_rate        != 0;
  (*dst)[ 2] = src->modulator.sustain_level     != 0;
  (*dst)[ 3] = src->modulator.release_rate      != 0;
  (*dst)[ 4] = src->modulator.waveform_type     != 0;
  (*dst)[ 5] = src->modulator.output_level      != 0;
  (*dst)[ 6] = src->modulator.key_scaling_level != 0;
  (*dst)[ 7] = src->modulator.multiplier        != 0;
  (*dst)[ 8] = src->modulator.tremolo           != 0;
  (*dst)[ 9] = src->modulator.vibrato           != 0;
  (*dst)[10] = src->modulator.key_scale_rate    != 0;
  (*dst)[11] = src->modulator.sustain           != 0;
  (*dst)[12] = src->carrier.attack_rate         != 0;
  (*dst)[13] = src->carrier.decay_rate          != 0;
  (*dst)[14] = src->carrier.sustain_level       != 0;
  (*dst)[15] = src->carrier.release_rate        != 0;
  (*dst)[16] = src->carrier.waveform_type       != 0;
  (*dst)[17] = src->carrier.output_level        != 0;
  (*dst)[18] = src->carrier.key_scaling_level   != 0;
  (*dst)[19] = src->carrier.multiplier          != 0;
  (*dst)[20] = src->carrier.tremolo             != 0;
  (*dst)[21] = src->carrier.vibrato             != 0;
  (*dst)[22] = src->carrier.key_scale_rate      != 0;
  (*dst)[23] = src->carrier.sustain             != 0;
  (*dst)[24] = src->connection_type             != 0;
  (*dst)[25] = src->feedback                    != 0;
  (*dst)[26] = src->frequency_slide             != 0;
  (*dst)[27] = src->panning                     != 0;
}
