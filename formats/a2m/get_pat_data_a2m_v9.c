// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_pat_data_a2m_v9 (tRAW_PATTERN_DATA *dst, pat_data_a2m_v9_t *src)
{
  memset (dst, 0, sizeof (*dst));

  for (unsigned line = 0; line < PATTERN_LEN_A2M_V9; line++)
    for (unsigned chan = 0; chan < CHANNELS_MAX_A2M_V9; chan++)
      get_pat_event_a2m_v9 (&(*dst)[chan][line], &(*src)[chan][line]);
}
