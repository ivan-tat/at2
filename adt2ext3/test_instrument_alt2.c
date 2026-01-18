// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
void test_instrument_alt2 (temp_instrument_t *dst, __UNUSED uint8_t chan, ExtKeyCode fkey)
{
  uint8_t octave; // 0..7
  int_least8_t board_pos; // 0..28, < 0 if not found
  String_t fname, basename;
  char ext[255+1];
  uint8_t channels;
  char *error;
  int8_t loader_status;

  DBG_ENTER ("test_instrument_alt2");

  if (scankey (SC_F7))
    for (uint8_t i = 0; i < 20; i++) reset_chan_data (i + 1);

  if (shift_pressed ()) goto _exit;

  octave = current_octave - 1;
  board_pos = board_get_pos (octave, fkey);
  if (board_pos < 0) goto _exit;

  {
    String_t s, t;

    CopyString ((String *)&fname, mn_environment.curr_item, 255);
    basename = BaseNameOnly ((String *)&fname);
    s = ExtOnly ((String *)&fname);
    t = Lower ((String *)&s);
    StringToStr (ext, (String *)&t, sizeof (ext) - 1);
  }

#if GO32
  keyboard_toggle_sleep ();
#endif // GO32

  if      (strcmp (ext, "bnk") == 0) loader_status = bnk_file_loader_alt (dst, instdata_source, Str2num ((String *)&basename, 10), &error);
  else if (strcmp (ext, "fib") == 0) loader_status = fib_file_loader_alt (dst, instdata_source, Str2num ((String *)&basename, 10), &error);
  else if (strcmp (ext, "ibk") == 0) loader_status = ibk_file_loader_alt (dst, instdata_source, Str2num ((String *)&basename, 10), &error);
  else
    loader_status = 1;

#if GO32
  keyboard_toggle_sleep ();
#endif // GO32
  if (loader_status != 0) goto _exit;

  if (Empty (&dst->ins1.fm, sizeof (dst->ins1.fm))) goto _exit;

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
  if (songdata.flag_4op != 0) songdata.flag_4op = 0;
  channels = percussion_mode ? 15 : 18;

  reset_player ();
  memset (chan_handle, 0, sizeof (chan_handle));
  memcpy (pan_lock_backup, pan_lock, sizeof (pan_lock_backup));
  memcpy (volume_lock_backup, volume_lock, sizeof (volume_lock_backup));
  memcpy (peak_lock_backup, peak_lock, sizeof (volume_lock_backup));
  memcpy (panning_table_backup, panning_table, sizeof (panning_table_backup));
  memset (pan_lock, false, sizeof (pan_lock));
  memset (volume_lock, false, sizeof (volume_lock));
  memset (peak_lock, false, sizeof (peak_lock));
  memcpy (fmpar_table_backup, fmpar_table, sizeof (fmpar_table_backup));
  memcpy (volume_table_backup, volume_table, sizeof (volume_table_backup));
  memcpy (freq_table_backup, freq_table, sizeof (freq_table_backup));
  memcpy (freqtable2_backup, freqtable2, sizeof (freqtable2_backup));
  memcpy (keyoff_loop_backup, keyoff_loop, sizeof (keyoff_loop_backup));
  memset (keyoff_loop, false, sizeof (keyoff_loop));

  misc_register = (current_tremolo_depth << 7)
                + (current_vibrato_depth << 6)
                + (percussion_mode << 5);
  key_off (17);
  key_off (18);
  opl2out (_instr[11], misc_register);

  if (   percussion_mode
      && (dst->ins1.fm.perc_voice >= 1)
      && (dst->ins1.fm.perc_voice <= 5))
  {
    uint8_t key = board_scancodes[board_pos];
    int note = board_pos + 12 * octave;

    if (note <= 12*8)
    {
      chan_handle[dst->ins1.fm.perc_voice - 1 + 15] = key;
      test_instrument_alt_output_note (dst, dst->ins1.fm.perc_voice - 1 + 15, note, current_inst - 1, -1);
    }

    while (scankey (key))
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
      int note = 12 * octave;

      valid_key = false;

      for (board_pos = 0; board_pos < 29; board_pos++, note++)
      {
        uint8_t key = board_scancodes[board_pos];
        bool pressed = scankey (key);
        int_least8_t chan1, chan2; // 0..19, (-1 if none)

        chan1 = get_chanpos (chan_handle, channels, key) - 1;
        chan2 = get_chanpos (chan_handle, channels, 0) - 1;

        if (pressed)
        {
          valid_key = true;

          if ((chan1 < 0) && (chan2 >= 0) && (note <= 12*8))
          {
            chan_handle[chan2] = key;
            test_instrument_alt_output_note (dst, chan2, note, current_inst - 1, -1);
          }
        }
        else if (chan1 >= 0)
        {
          chan_handle[chan1] = 0;
          key_off (chan1 + 1);
          if (alt_pressed ()) keyoff_loop[chan1] = true;
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
  memcpy (peak_lock, peak_lock_backup, sizeof (peak_lock));

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
