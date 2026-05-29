// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_ins_macro_a2f_v1 (tREGISTER_TABLE *dst, ins_macro_a2f_v1_t *src)
{
  get_ins_macro_a2m_v9 (dst, src);
  dst->arpeggio_table = 0;
  dst->vibrato_table  = 0;
}
