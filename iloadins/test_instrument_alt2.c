// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void test_instrument_alt2 (uint8_t chan, AT2ExtKeyCode fkey)
{
  uint8_t tone;
  String_t fname, basename, ext;
  uint8_t channels;

  DBG_ENTER ("test_instrument_alt2");

  if (scankey (SC_F7))
    for (uint8_t i = 0; i < 20; i++) reset_chan_data (i + 1);

  if (shift_pressed ()) goto _exit;

  {
    bool valid_key = false;

    for (uint_least8_t i = 0, note = 12 * (current_octave - 1); i < 29; i++, note++)
      if (board_scancodes[i] == (fkey >> 8))
      {
        if ((note < 0) || (note > 12*8+1)) goto _exit;
        tone = i;
        valid_key = true;
        break;
      }

    if (!valid_key) goto _exit;
  }

  channels = percussion_mode ? 15 : 18;

  load_flag_alt = UINT8_NULL;

  if (Empty (&temp_instrument, sizeof (temp_instrument))) goto _exit;

  {
    String_t t;

    CopyString ((String *)&fname, mn_environment.curr_item, 255);
    basename = BaseNameOnly ((String *)&fname);
    t = ExtOnly ((String *)&fname);
    ext = Lower ((String *)&t);
  }

#if GO32
  keyboard_toggle_sleep ();
#endif // GO32

  // Loader may change `load_flag_alt'
  if      (memcmp (&ext, (String *)"\x03" "bnk", 4) == 0) bnk_file_loader_alt (Str2num ((String *)&basename, 10));
  else if (memcmp (&ext, (String *)"\x03" "fib", 4) == 0) fib_file_loader_alt (Str2num ((String *)&basename, 10));
  else if (memcmp (&ext, (String *)"\x03" "ibk", 4) == 0) ibk_file_loader_alt (Str2num ((String *)&basename, 10));

#if GO32
  keyboard_toggle_sleep ();
#endif // GO32
  if (load_flag_alt == UINT8_NULL) goto _exit;

  ins_trailing_flag = true;
  status_backup.replay_forbidden = replay_forbidden;
  status_backup.play_status = play_status;
  replay_forbidden = true;
  if (play_status != isStopped) play_status = isPaused;
  nul_volume_bars ();
  really_no_status_refresh = true;
  reset_player ();

  memcpy (channel_flag_backup, channel_flag, sizeof (channel_flag_backup));
  memcpy (event_table_backup, event_table, sizeof (event_table_backup));
  common_flag_backup = songdata.common_flag;
  volume_scaling_backup = volume_scaling;
  songdata.common_flag &= ~0x80;
  volume_scaling = false;
  memset (channel_flag, true, sizeof (channel_flag));
  flag_4op_backup = songdata.flag_4op;
  if (!percussion_mode && (songdata.flag_4op == 0))
    channels = 18;
  else if (songdata.flag_4op == 0)
    channels = 15;
  else
  {
    songdata.flag_4op = 0;
    channels = percussion_mode ? 15 : 18;
  }

  reset_player ();
  memset (chan_handle, 0, sizeof (chan_handle));
  memcpy (pan_lock_backup, pan_lock, sizeof (pan_lock));
  memcpy (volume_lock_backup, volume_lock, sizeof (volume_lock));
  memcpy (peak_lock_backup, peak_lock, sizeof (volume_lock));
  memcpy (panning_table_backup, panning_table, sizeof (panning_table));
  memset (pan_lock, 0, sizeof (pan_lock));
  memset (volume_lock, 0, sizeof (volume_lock));
  memset (peak_lock, 0, sizeof (volume_lock));
  memcpy (fmpar_table_backup, fmpar_table, sizeof (fmpar_table_backup));
  memcpy (volume_table_backup, volume_table, sizeof (volume_table_backup));
  memcpy (freq_table_backup, freq_table, sizeof (freq_table));
  memcpy (freqtable2_backup, freqtable2, sizeof (freqtable2));
  memcpy (keyoff_loop_backup, keyoff_loop, sizeof (keyoff_loop));
  memset (keyoff_loop, false, sizeof (keyoff_loop));

  misc_register = (current_tremolo_depth << 7)
                + (current_vibrato_depth << 6)
                + (percussion_mode << 5);
  key_off (17);
  key_off (18);
  opl2out (_instr[11], misc_register);

  if (   percussion_mode
      && (temp_instrument.perc_voice >= 1)
      && (temp_instrument.perc_voice <= 5))
  {
    test_instrument_alt2_output_note (temp_instrument.perc_voice + 15, tone + 1);
    while (scankey (board_scancodes[tone]))
    {
#if GO32
      realtime_gfx_poll_proc ();
#else // !GO32
      _draw_screen_without_delay = true;
      keyboard_poll_input ();
#endif // !GO32
      keyboard_reset_buffer ();
      draw_screen ();
    }
  }
  else
  {
    bool valid_key;

    do
    {
      valid_key = false;

      for (uint_least8_t i = 0; i < 29; i++)
      {
        uint8_t key = board_scancodes[i];
        bool pressed = scankey (key);
        uint8_t pos1, pos2;

        pos1 = get_chanpos (chan_handle, channels, key);
        pos2 = get_chanpos (chan_handle, channels, 0);

        if (pressed)
        {
          valid_key = true;
          if ((pos1 == 0) && (pos2 != 0))
            test_instrument_alt2_output_note (pos2, i + 1);
        }
        else if (pos1 != 0)
        {
          chan_handle[pos1 - 1] = 0;
          key_off (pos1);
          if (alt_pressed ()) keyoff_loop[pos1 - 1] = true;
        }
      }

#if GO32
      realtime_gfx_poll_proc ();
#else // !GO32
      _draw_screen_without_delay = true;
      keyboard_poll_input ();
#endif // !GO32
      keyboard_reset_buffer ();
      draw_screen ();
    } while (valid_key);
  }

  while (ctrl_pressed ())
  {
#if GO32
    realtime_gfx_poll_proc ();
#else // !GO32
    _draw_screen_without_delay = true;
    keyboard_poll_input ();
#endif // !GO32
    keyboard_reset_buffer ();
    draw_screen ();
  }

  for (uint_least8_t i = 0; i < 20; i++) key_off (i + 1);

  songdata.flag_4op = flag_4op_backup;
  memcpy (fmpar_table, fmpar_table_backup, sizeof (fmpar_table));
  memcpy (volume_table, volume_table_backup, sizeof (volume_table));
  memcpy (panning_table, panning_table_backup, sizeof (panning_table));
  songdata.common_flag = common_flag_backup;
  volume_scaling = volume_scaling_backup;
  if (status_backup.play_status == isPlaying) reset_player ();

  memcpy (channel_flag, channel_flag_backup, sizeof (channel_flag));
  memcpy (event_table, event_table_backup, sizeof (event_table));
  memcpy (pan_lock, pan_lock_backup, sizeof (pan_lock));
  memcpy (volume_lock, volume_lock_backup, sizeof (volume_lock));
  memcpy (peak_lock, peak_lock_backup, sizeof (volume_lock));

  really_no_status_refresh = false;
  memcpy (freq_table, freq_table_backup, sizeof (freq_table));
  memcpy (freqtable2, freqtable2_backup, sizeof (freqtable2));
  memcpy (keyoff_loop, keyoff_loop_backup, sizeof (keyoff_loop));
  memset (macro_table, 0, sizeof (macro_table));
  replay_forbidden = status_backup.replay_forbidden;
  play_status = status_backup.play_status;
  ins_trailing_flag = false;
  keyboard_reset_buffer ();

_exit:
  DBG_LEAVE (); //EXIT //test_instrument_alt2
}
