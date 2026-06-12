// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_ins_data_cff (tADTRACK2_INS *dst, ins_data_cff_t *src)
{
  get_ins_fm_data_cff (&dst->fm_data, &src->fm_data);
  dst->panning    = 0; // N/A
  dst->fine_tune  = 0; // N/A
  dst->perc_voice = 0; // N/A
}
