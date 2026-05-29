// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In: `idx' = 0..INSTRUMENTS_MAX-1
static void put_ins_data_a2m_v9 (ins_data_a2m_v9_t *dst, tFIXED_SONGDATA *src, unsigned idx)
{
  tADTRACK2_INS *p = &src->instr_data[idx];

  put_ins_fm_data_a2m_v1 (&dst->fm_data, &p->fm_data);
  dst->panning    = p->panning;
  dst->fine_tune  = p->fine_tune;
  dst->voice_type = p->perc_voice;
}
