// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
/*static*/ uint16_t calc_vibtrem_shift (uint8_t chan, void *table_data) {
  tVIBRATO_TREMOLO_TABLE *t = table_data;
  uint8_t pos = (*t)[chan - 1].pos;

  (*t)[chan - 1].dir = (pos & vibtrem_table_size) ? 1 : 0;

  return (vibtrem_table[pos & (vibtrem_table_size - 1)]
          * (*t)[chan - 1].depth) >> 7;
}
