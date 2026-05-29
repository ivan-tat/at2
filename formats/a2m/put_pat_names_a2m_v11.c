// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In: `count' = 1..PATTERNS_MAX_A2M_V9
static void put_pat_names_a2m_v11 (pat_name_a2m_v11_t *dst, tFIXED_SONGDATA *src, unsigned count)
{
  for (unsigned i = 0; i < count; i++)
    put_string_a2m (&dst[i], sizeof (dst[0]), src->pattern_names[i], sizeof (src->pattern_names[0]), 11, '\0'); // skip first 11 characters
}
