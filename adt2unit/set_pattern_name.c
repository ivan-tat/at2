// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In:
//   * `idx' = 0..PATTERNS_MAX-1
//   * `name' may be NULL.
void set_pattern_name (tFIXED_SONGDATA *song, uint8_t idx, String *name, String *def_name)
{
  String_t s = Copy (song->pattern_names[idx], 1, 11); // keep first 11 characters

  AppendString ((String *)&s,
                name != NULL && Length (name) != 0 ? name : def_name,
                sizeof (song->pattern_names[0]) - 1);
  CopyString (song->pattern_names[idx], (String *)&s, sizeof (song->pattern_names[0]) - 1);
}
