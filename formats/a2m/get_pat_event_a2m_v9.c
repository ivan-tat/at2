// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_pat_event_a2m_v9 (tCHUNK *dst, pat_event_a2m_v9_t *src)
{
  dst->note        = src->note;
  dst->instr_def   = src->ins;
  dst->effect_def  = src->cmd1;
  dst->effect      = src->data1;
  dst->effect_def2 = src->cmd2;
  dst->effect2     = src->data2;
}
