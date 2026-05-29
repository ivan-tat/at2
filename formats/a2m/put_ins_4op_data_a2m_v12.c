// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void put_ins_4op_data_a2m_v12 (ins_4op_data_a2m_v12_t *dst, tFIXED_SONGDATA *src)
{
  dst->count = src->ins_4op_flags.num_4op;
  for (uint8_t i = 0; i < lengthof (dst->indexes, [0]); i++)
    dst->indexes[i] = src->ins_4op_flags.idx_4op[i];
}
