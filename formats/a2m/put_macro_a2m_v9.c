// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In: `idx' = 0..INSTRUMENTS_MAX-1
static void put_macro_a2m_v9 (macro_a2m_v9_t *dst, tFIXED_SONGDATA *src, unsigned idx)
{
  tMACRO_TABLE *p = &src->macro_table[idx];

  put_arp_macro_a2m_v9 (&dst->arpeggio, &p->arpeggio);
  put_vib_macro_a2m_v9 (&dst->vibrato, &p->vibrato);
}
