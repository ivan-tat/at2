// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void put_pat_order_a2m_v1 (pat_order_a2m_v1_t *dst, tFIXED_SONGDATA *src)
{
  memcpy (dst, src->pattern_order, sizeof (*dst));
}
