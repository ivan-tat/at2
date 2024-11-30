// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void count_patterns (uint8_t *patterns)
{
  uint8_t n = max_patterns;

  DBG_ENTER ("count_patterns");

  while (n && is_data_empty (&(*pattdata)[(n - 1) / 8][(n - 1) % 8], PATTERN_SIZE))
    n--;

  *patterns = n;

  DBG_LEAVE (); //EXIT //count_patterns
}
