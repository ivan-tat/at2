// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void calibrate_player (uint8_t order, uint8_t line,
                       bool status_filter, bool line_dependent)
{
  bool temp_channel_flag[20];
  bool old_debugging,
       old_repeat_pattern;
  uint8_t previous_order, previous_line;
  struct
  {
    bool replay_forbidden;
    tPLAY_STATUS play_status;
  } status_backup;

  uint8_t _pattern_page, _pattord_page,
          _pattord_hpos, _pattord_vpos;

  DBG_ENTER ("calibrate_player");

  if (calc_following_order (0) == -1)
  {
    DBG_LEAVE (); //calibrate_player
    return;
  }
  calibrating = true;
  status_backup.replay_forbidden = replay_forbidden;
  status_backup.play_status = play_status;
  if (status_filter)
    no_status_refresh = true;

  nul_volume_bars ();
  memmove (temp_channel_flag, channel_flag, sizeof (temp_channel_flag));
  memset (channel_flag, false, sizeof (channel_flag));

  old_debugging = debugging;
  old_repeat_pattern = repeat_pattern;
  debugging = false;
  repeat_pattern = false;

  if (   (play_status == isStopped)
      || (order < current_order)
      || (order == calc_following_order (0)))
  {
    bool nsp = no_sync_playing;

    stop_playing ();
    if (nsp)
      no_sync_playing = true;

    init_player ();
    speed = songdata.speed;
    macro_speedup = songdata.macro_speedup;
    update_timer (songdata.tempo);
    current_order = calc_following_order (0);
    current_pattern = songdata.pattern_order[current_order];
    current_line = 0;
    pattern_break = false;
    pattern_delay = false;
    last_order = 0;
    next_line = 0;
    song_timer = 0;
    timer_temp = 0;
    song_timer_tenths = 0;
    time_playing = 0;
    ticklooper = 0;
    macro_ticklooper = 0;
    ticks = 0;
    tick0 = 0;
  }

  if (!no_sync_playing)
  {
    show_str (13, 7, (String *)"\x09" " --:--.- ", status_background + status_border);
#if !GO32
    _draw_screen_without_delay = true;
#endif // !GO32
    draw_screen ();
  }

  previous_order = current_order;
  previous_line = current_line;
  replay_forbidden = true;
  seek_pattern_break = false;

  if (!no_sync_playing)
  {
    uint8_t jump_count = 0;
    uint8_t loop_count = 0;

    while (   (current_line != line)
           || (current_order != order))
    {
      if (scankey (SC_ESCAPE))
        break;
      if (!(   (previous_order == current_order)
            && (previous_line >= current_line)
            && !(   pattern_break
                 && ((next_line & 0xF0) == pattern_loop_flag))))
        loop_count = 0;
      else
      {
        loop_count++;
        if (loop_count >= 16)
          break;
      }

      if (   (current_order == order)
          && (current_line >= line)
          && !line_dependent)
        break;

      previous_order = current_order;
      previous_line = current_line;
      fast_forward = true;

      poll_proc ();
      if (!macro_ticklooper)
        macro_poll_proc ();

      fast_forward = false;
      ticklooper++;
      if (ticklooper >= (IRQ_freq / tempo))
        ticklooper = 0;

      macro_ticklooper++;
      if (macro_ticklooper >= (IRQ_freq / (tempo * macro_speedup)))
        macro_ticklooper = 0;

      if (previous_order != current_order)
      {
        update_status ();
        jump_count++;
        if (jump_count >= 0x80)
          break;
      }

      if (seek_pattern_break)
      {
        if (       (current_order > order)
            || (   (current_order == order)
                && (current_line || !line)))
          break;
        else
          seek_pattern_break = false;
      }

      keyboard_reset_buffer ();
    }
  }
  else
  {
    start_playing ();
    current_order = order;
    current_pattern = songdata.pattern_order[order];
    current_line = line;
  }

  fade_out_volume = 63;
  memmove (channel_flag, temp_channel_flag, sizeof (channel_flag));

  for (uint8_t chan = 0; chan < 20; chan++)
    reset_chan_data (chan + 1);

  if (status_backup.play_status != isStopped)
  {
    replay_forbidden = status_backup.replay_forbidden;
    play_status = status_backup.play_status;
  }
  else
  {
    replay_forbidden = false;
    play_status = isPlaying;
  }

  debugging = old_debugging;
  repeat_pattern = old_repeat_pattern;
  synchronize_song_timer ();
  calibrating = false;
  if (status_filter)
    no_status_refresh = false;

  _pattern_page = line;
  _pattord_page = 0;
  _pattord_hpos = 1;
  _pattord_vpos = 1;

  while (current_order != (_pattord_vpos + 4 * (_pattord_hpos + _pattord_page - 1) - 1))
    if (_pattord_vpos < 4)
      _pattord_vpos++;
    else if (_pattord_hpos < MAX_ORDER_COLS)
    {
      _pattord_hpos++;
      _pattord_vpos = 1;
    }
    else if (_pattord_page < (23 - (MAX_ORDER_COLS - 9)))
    {
      _pattord_page++;
      _pattord_vpos = 1;
    }

  if (tracing)
  {
    PATTERN_ORDER_page_refresh (_pattord_page);
    PATTERN_page_refresh (_pattern_page);
  }

  keyboard_reset_buffer ();

  DBG_LEAVE (); //EXIT //calibrate_player
}
