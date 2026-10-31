// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// idx: 0..PATTERNS_MAX-1
// Returns `true' on success and `false' on error.
bool pattern_avail (UNUSED tFIXED_SONGDATA *song, uint8_t idx)
{
  return idx < max_patterns;
}
