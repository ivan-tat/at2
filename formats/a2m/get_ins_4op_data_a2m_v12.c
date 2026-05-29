// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_ins_4op_data_a2m_v12 (tFIXED_SONGDATA *dst, ins_4op_data_a2m_v12_t *src)
{
  dst->ins_4op_flags.num_4op = src->count;
  for (unsigned i = 0; i < lengthof (dst->ins_4op_flags.idx_4op, [0]); i++)
    dst->ins_4op_flags.idx_4op[i] = src->indexes[i];
}
