// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// `marks' may be NULL.
static void _restore_pattern_marks (tFIXED_SONGDATA *song, const uint8_t *marks, unsigned count)
{
  for (unsigned i = 0; i < count; i++)
  {
    char c = (marks != NULL) && bit_test (marks, i) ? charmap.black_right_triangle : ' ';

    if (Length (song->pattern_names[i]) >= 11)
      GetStr (song->pattern_names[i])[0] = c;
  }
}
