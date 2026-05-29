// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In: `idx' = 0..INSTRUMENTS_MAX-1.
static void put_ins_macro_a2f_v1 (ins_macro_a2f_v1_t *dst,
                                  tFIXED_SONGDATA *src, unsigned idx)
{
  put_ins_macro_a2m_v9 (dst, src, idx);
  dst->arpeggio_table = 0;
  dst->vibrato_table  = 0;
}
