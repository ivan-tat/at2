// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_flags_a2m_v9 (tFIXED_SONGDATA *dst, uint8_t *src)
{
  dst->common_flag = *src;
}
