// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_ins_data_a2m_v9 (tADTRACK2_INS *dst, ins_data_a2m_v9_t *src)
{
  get_ins_fm_data_a2m_v1 (&dst->fm_data, &src->fm_data);
  dst->panning    = src->panning;
  dst->fine_tune  = src->fine_tune;
  dst->perc_voice = src->voice_type;
}
