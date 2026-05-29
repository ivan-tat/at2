// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void put_chan_flags_a2m_v13 (chan_flags_a2m_v10_t *dst, tFIXED_SONGDATA *src)
{
  for (unsigned i = 0; i < lengthof ((*dst), [0]); i++)
    (*dst)[i] = src->lock_flags[i] & 0x7F;
}
