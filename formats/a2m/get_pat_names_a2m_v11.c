// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In: `count' = 1..PATTERNS_MAX_A2M_V9
static void get_pat_names_a2m_v11 (tFIXED_SONGDATA *dst, pat_name_a2m_v11_t *src, unsigned count)
{
  for (unsigned i = 0; i < count; i++)
    AppendString (dst->pattern_names[i], (String *)&src[i], sizeof (dst->pattern_names[0]) - 1);

/*
  for (unsigned i = count; i < PATTERNS_MAX; i++)
    SetLength (song->pattern_names[i], 0);
*/
}
