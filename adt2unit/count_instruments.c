// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void count_instruments (uint8_t *instruments)
{
  uint8_t n = 255;

  DBG_ENTER ("count_instruments");

  while (n && is_data_empty (&songdata.instr_data[n - 1], INSTRUMENT_SIZE))
    n--;

  *instruments = n;

  DBG_LEAVE (); //EXIT //count_instruments
}
