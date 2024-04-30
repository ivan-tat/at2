// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void init_StringIO (void) {
  int i;

  // Setup all glyphs as '?'
  memset (UCS2_to_AT2_ct, '?', sizeof (UCS2_to_AT2_ct));

  // Setup first 128 glyphs
  for (i = 0; i < 128; i++)
    UCS2_to_AT2_ct[i] = i;

  // Setup CP437
  for (i = 0; CP437_it[i].glyph >= 0; i++)
    UCS2_to_AT2_ct[CP437_it[i].uc] = CP437_it[i].glyph;
}
