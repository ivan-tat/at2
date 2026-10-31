// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// idx: 0..PATTERNS_MAX-1
// Returns `true' on success and `false' on error.
bool touch_pattern (UNUSED tFIXED_SONGDATA *song, uint8_t idx)
{
  if (idx < max_patterns)
  {
#if !ADT2PLAY
    module_archived = false;
#endif // !ADT2PLAY
    return true;
  }
  else
  {
    limit_exceeded = true;
    return false;
  }
}
