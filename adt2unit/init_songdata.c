// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void init_songdata (tFIXED_SONGDATA *song)
{
  DBG_ENTER ("init_songdata");

  if (play_status != isStopped)
  {
#if !ADT2PLAY
    fade_out_playback (false);
#endif // !ADT2PLAY
    stop_playing ();
  }
  else
    init_buffers ();

  memset (song, 0, sizeof (*song));
  memset (&song->pattern_order, PATTERN_ORDER_JUMP + 0, sizeof (song->pattern_order));
  memset (pattdata, 0, PATTERN_SIZE * max_patterns);

#if ADT2PLAY
  song->patt_len = 64;
  song->nm_tracks = 9;
#else // !ADT2PLAY
  song->patt_len = patt_len;
  song->nm_tracks = nm_tracks;
#endif // !ADT2PLAY
  song->tempo = tempo;
  song->speed = speed;
#if ADT2PLAY
  song->macro_speedup = 1;
#else // !ADT2PLAY
  song->macro_speedup = init_macro_speedup;
#endif // !ADT2PLAY
  speed_update = false;
#if !ADT2PLAY
  IRQ_freq_shift = 0;
  playback_speed_shift = 0;
  song->bpm_data.rows_per_beat = mark_line;
#endif // !ADT2PLAY
  song->bpm_data.tempo_finetune = IRQ_freq_shift;
  lockvol = false;
  panlock = false;
  lockVP  = false;
  tremolo_depth = 0;
  vibrato_depth = 0;
  volume_scaling = false;

#if !ADT2PLAY
  old_chan_pos = 1;
  old_hpos = 1;
  old_page = 0;
  old_block_chan_pos = 1;
  old_block_patt_hpos = 1;
  old_block_patt_page = 0;
  marking = false;
#endif // !ADT2PLAY

  if (song->nm_tracks <= MELODIC_CHANNELS_MAX)
  {
    percussion_mode = false;
    memcpy (_chan_n, _chmm_n, sizeof (_chan_n));
    memcpy (_chan_m, _chmm_m, sizeof (_chan_m));
    memcpy (_chan_c, _chmm_c, sizeof (_chan_c));
  }
  else
  {
    percussion_mode = true;
    memcpy (_chan_n, _chpm_n, sizeof (_chan_n));
    memcpy (_chan_m, _chpm_m, sizeof (_chan_m));
    memcpy (_chan_c, _chpm_c, sizeof (_chan_c));
  }

#if !ADT2PLAY
  for (unsigned i = 0; i < INSTRUMENTS_MAX; i++)
  {
    SetLength (song->instr_names[i], 9);
    snprintf (GetStr ((char *)song->instr_names[i]), 9+1,
              " iNS_%02"PRIX8"%c ", i + 1, /*charmap.*/'\xF7');
  }

  for (unsigned i = 0; i < PATTERNS_MAX; i++)
  {
    SetLength (song->pattern_names[i], 11);
    snprintf (GetStr ((char *)song->pattern_names[i]), 11+1,
              " PAT_%02"PRIX8"  %c ", i + 1, /*charmap.*/'\xF7');
  }

  songdata_crc_ord = Update32 (song->pattern_order,
                               sizeof (song->pattern_order), /*CRC32_INITVAL*/ 0);
  module_archived = true;
#endif // !ADT2PLAY

  DBG_LEAVE (); //EXIT //init_songdata
}
