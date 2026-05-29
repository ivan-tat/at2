// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In: `idx' = 0..PATTERNS_MAX-1
bool is_pattern_marked (tFIXED_SONGDATA *song, uint8_t idx)
{
  return GetStr (song->pattern_names[idx])[0] == charmap.black_right_triangle;
}
