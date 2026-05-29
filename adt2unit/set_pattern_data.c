// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In:
//   * `idx' = 0..PATTERNS_MAX-1
//   * `data' may be NULL.
void set_pattern_data (UNUSED tFIXED_SONGDATA *song, uint8_t idx, tRAW_PATTERN_DATA *data)
{
  if (idx < max_patterns)
  {
    if (data != NULL)
      memcpy (&(*pattdata)[idx / 8][idx % 8], data, sizeof ((*pattdata)[0][0]));
    else
      memset (&(*pattdata)[idx / 8][idx % 8], 0, sizeof ((*pattdata)[0][0]));
  }
}
