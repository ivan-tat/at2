// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void put_chan_4op_flags_a2m_v10 (uint8_t *dst, tFIXED_SONGDATA *src)
{
  *dst = src->flag_4op & 0x3F;
}
