// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_chan_flags_a2m_v10 (tFIXED_SONGDATA *dst, chan_flags_a2m_v10_t *src)
{
  for (unsigned i = 0; i < lengthof ((*src), [0]); i++)
    dst->lock_flags[i] = (*src)[i] & 0x3F;
}
