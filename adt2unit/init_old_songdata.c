// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void init_old_songdata (void)
{
  DBG_ENTER ("init_old_songdata");

  memset (&old_songdata, 0, sizeof (old_songdata));
  memset (old_songdata.pattern_order, 0x80, sizeof (old_songdata.pattern_order));
  //memset (old_songdata.instr_data, 0, sizeof (old_songdata.instr_data)); // already cleared

#if !ADT2PLAY
  for (uint8_t i = 0; i < 250; i++)
  {
    SetLength (old_songdata.instr_names[i], 9);
    snprintf (GetStr ((char *)old_songdata.instr_names[i]), 9+1, " iNS_%02"PRIX8"%c ", i + 1, /*charmap.*/'\xF7');
  }
#endif // !ADT2PLAY

  DBG_LEAVE (); //EXIT //init_old_songdata
}
