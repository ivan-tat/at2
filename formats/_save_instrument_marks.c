// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void _save_instrument_marks (tFIXED_SONGDATA *song, uint8_t *marks, unsigned count)
{
  bit_clear_range (marks, 0, count - 1);

  for (unsigned i = 0; i < count; i++)
    if (GetStr (song->instr_names[i])[0] == charmap.black_right_triangle)
      bit_set (marks, i);
}
