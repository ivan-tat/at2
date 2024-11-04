// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// pattern: 0..max_patterns-1
// line: 0..255
// channel: 1..20
void put_chunk (uint8_t pattern, uint8_t line, uint8_t channel, const tCHUNK *chunk)
{
  if (pattern < max_patterns)
  {
    (*pattdata)[pattern / 8][pattern % 8][channel - 1][line] = *chunk;
    module_archived = false;
  }
  else
    limit_exceeded = true;
}
