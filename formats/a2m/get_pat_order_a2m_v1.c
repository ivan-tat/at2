// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_pat_order_a2m_v1 (tFIXED_SONGDATA *dst, pat_order_a2m_v1_t *src)
{
  memcpy (dst->pattern_order, src, sizeof (dst->pattern_order));
}
