// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void start_playing (void)
{
  DBG_ENTER ("start_playing");

#if ADT2PLAY
  stop_playing ();
  if (error_code != 0)
  {
    DBG_LEAVE (); //start_playing
    return;
  }
#endif // ADT2PLAY

  init_player ();

#if ADT2PLAY
  current_order = 0;
  if (songdata.pattern_order[current_order] >= 0x80)
    if (calc_order_jump () == -1)
    {
      DBG_LEAVE (); //start_playing
      return;
    }

  current_pattern = songdata.pattern_order[current_order];
  current_line = 0;
#else // !ADT2PLAY
  if (start_pattern == BYTE_NULL)
    current_order = 0;
  else
    if (start_order == BYTE_NULL)
    {
      uint8_t start = calc_pattern_pos (start_pattern);

      if (start != BYTE_NULL)
        current_order = start;
      else
        if (!play_single_patt)
        {
          start_pattern = BYTE_NULL;
          current_order = 0;
          DBG_LEAVE (); //start_playing
          return;
        }
    }
    else
    {
      current_order = start_order;
      current_pattern = start_pattern;
    }

  if (!play_single_patt)
    if (songdata.pattern_order[current_order] >= 0x80)
      if (calc_order_jump () == -1)
      {
        DBG_LEAVE (); //start_playing
        return;
      }

  current_pattern = (!play_single_patt) ? songdata.pattern_order[current_order]
                                        : start_pattern;
  current_line = (start_line == BYTE_NULL) ? 0 : start_line;
#endif // !ADT2PLAY

  pattern_break = false;
  pattern_delay = false;
  tickXF = 0;
  ticks = 0;
  tick0 = 0;
#if !ADT2PLAY
  last_order = 0;
#endif // !ADT2PLAY
  next_line = 0;
  song_timer = 0;
  timer_temp = 0;
  song_timer_tenths = 0;
  time_playing = 0;
  ticklooper = 0;
  macro_ticklooper = 0;
#if !ADT2PLAY
  debugging = false;
  fade_out_volume = 63;
#endif // !ADT2PLAY
  playback_speed_shift = 0;
  replay_forbidden = false;
  play_status = isPlaying;
  speed = songdata.speed;
  macro_speedup = songdata.macro_speedup;
#if ADT2PLAY
  IRQ_freq_shift = songdata.bpm_data.tempo_finetune;
#endif // ADT2PLAY

  update_timer (songdata.tempo);

#if !ADT2PLAY
  no_status_refresh = false;
  really_no_status_refresh = false;
  memset (play_pos_buf, 0, sizeof (play_pos_buf));
#endif // !ADT2PLAY

  DBG_LEAVE (); //EXIT //start_playing
}
