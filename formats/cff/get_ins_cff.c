// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In: `idx' = 0..INSTRUMENTS_MAX-1
static void get_ins_cff (tFIXED_SONGDATA *dst, unsigned idx, ins_cff_t *src)
{
  get_ins_data_cff (&dst->instr_data[idx], &src->ins_data);
  get_ins_name_cff (&dst->instr_names[idx], &src->ins_name);
}
