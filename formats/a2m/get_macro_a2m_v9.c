// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_macro_a2m_v9 (tMACRO_TABLE *dst, macro_a2m_v9_t *src)
{
  get_arp_macro_a2m_v9 (&dst->arpeggio, &src->arpeggio);
  get_vib_macro_a2m_v9 (&dst->vibrato, &src->vibrato);
}
