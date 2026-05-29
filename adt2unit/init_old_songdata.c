// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void init_old_songdata (void)
{
  tOLD_FIXED_SONGDATA *song = &old_songdata;

  DBG_ENTER ("init_old_songdata");

  SetLength (song->songname, 0);
  SetLength (song->composer, 0);
#if !ADT2PLAY
  for (uint8_t i = 0; i < /*INSTRUMENTS_MAX_A2M_V1*/250; i++)
  {
    SetLength (song->instr_names[i], 9);
    snprintf (GetStr ((char *)song->instr_names[i]), 1 + 9, " iNS_%02"PRIX8"%c ", i + 1, /*charmap.*/'\xF7');
    memset (&song->instr_data[i], 0, sizeof (song->instr_data[0]));
  }
#endif // !ADT2PLAY
  memset (song->pattern_order, PATTERN_ORDER_JUMP + 0, sizeof (song->pattern_order));
  song->tempo = 0;
  song->speed = 0;
  song->common_flag = 0;

  DBG_LEAVE (); //EXIT //init_old_songdata
}
