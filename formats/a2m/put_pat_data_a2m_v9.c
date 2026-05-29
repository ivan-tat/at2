// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void put_pat_data_a2m_v9 (pat_data_a2m_v9_t *dst, tRAW_PATTERN_DATA *src)
{
  unsigned num_channels = max (CHANNELS_MAX_A2M_V9, CHANNELS_MAX);
  unsigned num_lines = max (PATTERN_LEN_A2M_V9, PATTERN_LEN);

  for (unsigned chan = 0; chan < num_channels; chan++)
    for (unsigned line = 0; line < num_lines; line++)
    {
      tCHUNK *src_ev = &(*src)[chan][line];
      pat_event_a2m_v9_t *dst_ev = &(*dst)[chan][line];

      dst_ev->note  = src_ev->note;
      dst_ev->ins   = src_ev->instr_def;
      dst_ev->cmd1  = src_ev->effect_def;
      dst_ev->data1 = src_ev->effect;
      dst_ev->cmd2  = src_ev->effect_def2;
      dst_ev->data2 = src_ev->effect2;
    }
}
