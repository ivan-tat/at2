// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*static*/ void import_old_a2m_patterns2 (uint8_t block, uint8_t count)
{
  uint8_t patt_count = count <= 8 ? count : 8;

  for (uint8_t patt = 0; patt < patt_count; patt++)
    for (uint8_t line = 0; line < 0x40; line++)
      for (uint8_t chan = 0; chan < 18; chan++)
      {
        tOLD_CHUNK chunk = hash_buffer[patt][chan][line];
        import_old_a2m_event2 (block * 8 + patt, line, chan + 1, chunk);
      }
}
