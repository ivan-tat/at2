// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#define ef_ManualFSlide 22

static void get_pat_event_a2m_v5 (tCHUNK *dst, pat_event_a2m_v1_t *src)
{
  memset (dst, 0, sizeof (*dst));
  dst->note      = src->note;
  dst->instr_def = src->ins;

  if (src->cmd != ef_ManualFSlide)
  {
    dst->effect_def = src->cmd;
    dst->effect     = src->data;
  }
  else if ((src->data >> 4) != 0)
  {
    dst->effect_def = ef_Extended2;
    dst->effect     = (ef_ex2_FineTuneUp << 4) + (src->data >> 4);
  }
  else
  {
    dst->effect_def = ef_Extended2;
    dst->effect     = (ef_ex2_FineTuneDown << 4) + (src->data & 0x0F);
  }
}
