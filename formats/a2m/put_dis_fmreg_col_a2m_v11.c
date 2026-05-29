// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In: `idx' = 0..INSTRUMENTS_MAX-1
static void put_dis_fmreg_col_a2m_v11 (dis_fmreg_col_a2m_v11_t *dst, tFIXED_SONGDATA *src, unsigned idx)
{
  tDIS_FMREG_COL *p = &src->dis_fmreg_col[idx];

  dst->modulator.attack_rate       = (*p)[ 0] ? 1 : 0;
  dst->modulator.decay_rate        = (*p)[ 1] ? 1 : 0;
  dst->modulator.sustain_level     = (*p)[ 2] ? 1 : 0;
  dst->modulator.release_rate      = (*p)[ 3] ? 1 : 0;
  dst->modulator.waveform_type     = (*p)[ 4] ? 1 : 0;
  dst->modulator.output_level      = (*p)[ 5] ? 1 : 0;
  dst->modulator.key_scaling_level = (*p)[ 6] ? 1 : 0;
  dst->modulator.multiplier        = (*p)[ 7] ? 1 : 0;
  dst->modulator.tremolo           = (*p)[ 8] ? 1 : 0;
  dst->modulator.vibrato           = (*p)[ 9] ? 1 : 0;
  dst->modulator.key_scale_rate    = (*p)[10] ? 1 : 0;
  dst->modulator.sustain           = (*p)[11] ? 1 : 0;
  dst->carrier.attack_rate         = (*p)[12] ? 1 : 0;
  dst->carrier.decay_rate          = (*p)[13] ? 1 : 0;
  dst->carrier.sustain_level       = (*p)[14] ? 1 : 0;
  dst->carrier.release_rate        = (*p)[15] ? 1 : 0;
  dst->carrier.waveform_type       = (*p)[16] ? 1 : 0;
  dst->carrier.output_level        = (*p)[17] ? 1 : 0;
  dst->carrier.key_scaling_level   = (*p)[18] ? 1 : 0;
  dst->carrier.multiplier          = (*p)[19] ? 1 : 0;
  dst->carrier.tremolo             = (*p)[20] ? 1 : 0;
  dst->carrier.vibrato             = (*p)[21] ? 1 : 0;
  dst->carrier.key_scale_rate      = (*p)[22] ? 1 : 0;
  dst->carrier.sustain             = (*p)[23] ? 1 : 0;
  dst->connection_type             = (*p)[24] ? 1 : 0;
  dst->feedback                    = (*p)[25] ? 1 : 0;
  dst->frequency_slide             = (*p)[26] ? 1 : 0;
  dst->panning                     = (*p)[27] ? 1 : 0;
}
