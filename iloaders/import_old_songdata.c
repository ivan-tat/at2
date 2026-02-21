// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void import_old_songdata (tOLD_FIXED_SONGDATA *old_songdata)
{
  tFIXED_SONGDATA *song = &songdata;

  CopyString (song->songname, old_songdata->songname, sizeof (song->songname) - 1);
  CopyString (song->composer, old_songdata->composer, sizeof (song->composer) - 1);

  for (uint8_t i = 0; i < 250; i++)
  {
    CopyString (song->instr_names[i], old_songdata->instr_names[i], sizeof (song->instr_names[0]) - 1);
    song->instr_data[i].fm_data = old_songdata->instr_data[i].fm_data;
    song->instr_data[i].panning = old_songdata->instr_data[i].panning;
    song->instr_data[i].fine_tune = old_songdata->instr_data[i].fine_tune;
    song->instr_data[i].perc_voice = 0;
  }

  memcpy (song->pattern_order, old_songdata->pattern_order, sizeof (old_songdata->pattern_order));

  song->tempo = old_songdata->tempo;
  song->speed = old_songdata->speed;
  song->common_flag = old_songdata->common_flag;
  import_old_flags ();
}
