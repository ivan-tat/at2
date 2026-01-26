// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// test_ins1: 1..255
// test_ins2: 1..255 (0 if none)
void test_instrument_alt (__UNUSED uint8_t chan, ExtKeyCode fkey,
                          bool loadMacros,
                          bool bankSelector,
                          bool loadArpVib,
                          uint8_t test_ins1, uint8_t test_ins2)
{
  uint8_t octave; // 0..7
  int_least8_t board_pos; // 0..28, < 0 if not found
  String_t fname;
  char ext[255+1];
  uint_least8_t channels;
  temp_instrument_t *temp_ins = &temp_instrument;
  char *error;

  DBG_ENTER ("test_instrument_alt");

  if (scankey (SC_F7))
    for (uint8_t i = 0; i < 20; i++) reset_chan_data (i + 1);

  if (shift_pressed ()) goto _exit;

  octave = current_octave - 1;
  board_pos = board_get_pos (octave, fkey);
  if (board_pos < 0) goto _exit;

  memset (&temp_ins->ins1.fm, 0, sizeof (temp_ins->ins1.fm));
  memset (&temp_ins->ins2.fm, 0, sizeof (temp_ins->ins2.fm));

  {
    String_t s, t;

    CopyString ((String *)&fname, mn_environment.curr_item, sizeof (fname) - 1);
    s = ExtOnly ((String *)&fname);
    t = Lower ((String *)&s);
    StringToStr (ext, (String *)&t, sizeof (ext) - 1);
  }

  if (bankSelector || loadArpVib)
  {
    tFIXED_SONGDATA *p;

    if (loadArpVib)
      p = &songdata;
    else
    {
      StrToString ((String *)&fname, "", sizeof (fname) - 1);
      p = &temp_songdata;
    }
    temp_ins->four_op = false;
    memcpy (&temp_ins->ins1.fm, &p->instr_data[test_ins1 - 1], sizeof (temp_ins->ins1.fm));
    memcpy (&temp_ins->ins1.macro, &p->instr_macros[test_ins1 - 1], sizeof (temp_ins->ins1.macro));
    memcpy (&temp_ins->ins1.dis_fmreg_col, &p->dis_fmreg_col[test_ins1 - 1], sizeof (temp_ins->ins1.dis_fmreg_col));
    if (test_ins2 != 0)
    {
      temp_ins->four_op = true;
      memcpy (&temp_ins->ins2.fm, &p->instr_data[test_ins2 - 1], sizeof (temp_ins->ins2.fm));
      memcpy (&temp_ins->ins2.macro, &p->instr_macros[test_ins2 - 1], sizeof (temp_ins->ins2.macro));
      memcpy (&temp_ins->ins2.dis_fmreg_col, &p->dis_fmreg_col[test_ins2 - 1], sizeof (temp_ins->ins2.dis_fmreg_col));
    }
  }
  else
  {
    int8_t loader_status;

#if GO32
    keyboard_toggle_sleep ();
#endif // GO32

    if      (strcmp (ext, "a2i") == 0) loader_status = a2i_file_loader_alt (temp_ins, (String *)&fname, true, NULL, &error);
    else if (strcmp (ext, "a2f") == 0) loader_status = a2f_file_loader_alt (temp_ins, (String *)&fname, true, NULL, &error);
    else if (strcmp (ext, "cif") == 0) loader_status = cif_file_loader_alt (temp_ins, (String *)&fname, &error);
    else if (strcmp (ext, "fin") == 0) loader_status = fin_file_loader_alt (temp_ins, (String *)&fname, &error);
    else if (strcmp (ext, "ins") == 0) loader_status = ins_file_loader_alt (temp_ins, (String *)&fname, &error);
    else if (strcmp (ext, "sbi") == 0) loader_status = sbi_file_loader_alt (temp_ins, (String *)&fname, &error);
    else if (strcmp (ext, "sgi") == 0) loader_status = sgi_file_loader_alt (temp_ins, (String *)&fname, &error);
    else
      loader_status = 1;

#if GO32
    keyboard_toggle_sleep ();
#endif // GO32
    if (loader_status != 0) goto _exit;
  }

  if (   (test_ins2 != 0)
      && Empty (&temp_ins->ins2.fm, sizeof (temp_ins->ins2.fm))
      && Empty (&temp_ins->ins2.macro, sizeof (temp_ins->ins2.macro)))
  {
    temp_ins->four_op = false;
    test_ins2 = 0;
  }

  memcpy (&arp_vib_macro_table_backup, &songdata.macro_table, sizeof (arp_vib_macro_table_backup));

  if (loadArpVib)
  {
    if (arp_tab_selected)
    {
      temp_ins->ins1.macro.arpeggio_table = arpvib_arpeggio_table;
      memcpy (&     songdata.macro_table[arpvib_arpeggio_table - 1].arpeggio,
              &temp_songdata.macro_table[arpvib_arpeggio_table - 1].arpeggio,
              sizeof (songdata.macro_table[0].arpeggio)); // HINT: was missing `.arpeggio'
    }
    else
      temp_ins->ins1.macro.arpeggio_table = 0;

    if (vib_tab_selected)
    {
      temp_ins->ins1.macro.vibrato_table = arpvib_vibrato_table;
      memcpy (&     songdata.macro_table[arpvib_vibrato_table - 1].vibrato,
              &temp_songdata.macro_table[arpvib_vibrato_table - 1].vibrato,
              sizeof (songdata.macro_table[0].vibrato)); // HINT: was missing `.vibrato'
    }
    else
      temp_ins->ins1.macro.vibrato_table = 0;

    memcpy (&songdata.instr_macros[test_ins1 - 1], &temp_ins->ins1.macro, sizeof (songdata.instr_macros[0]));
    if (test_ins2 != 0)
      memcpy (&songdata.instr_macros[test_ins2 - 1], &temp_ins->ins2.macro, sizeof (songdata.instr_macros[0]));
  }
  else if (bankSelector && loadMacros)
  {
    if (temp_ins->ins1.macro.arpeggio_table != 0)
      memcpy (&     songdata.macro_table[temp_ins->ins1.macro.arpeggio_table - 1].arpeggio,
              &temp_songdata.macro_table[temp_ins->ins1.macro.arpeggio_table - 1].arpeggio,
              sizeof (songdata.macro_table[0].arpeggio)); // HINT: was missing `.arpeggio'
    if (temp_ins->ins1.macro.vibrato_table != 0)
      memcpy (&     songdata.macro_table[temp_ins->ins1.macro.vibrato_table - 1].vibrato,
              &temp_songdata.macro_table[temp_ins->ins1.macro.vibrato_table - 1].vibrato,
              sizeof (songdata.macro_table[0].vibrato)); // HINT: was missing `.vibrato'
  }

  if ((   Empty (&temp_ins->ins1.fm, sizeof (temp_ins->ins1.fm))
       && Empty (&temp_ins->ins1.macro, sizeof (temp_ins->ins1.macro)))
      || (   (test_ins2 != 0)
          && Empty (&temp_ins->ins2.fm, sizeof (temp_ins->ins2.fm))
          && Empty (&temp_ins->ins2.macro, sizeof (temp_ins->ins2.macro))))
    goto _exit;

  ins_trailing_flag = true;
  status_backup.replay_forbidden = replay_forbidden;
  status_backup.play_status = play_status;
  replay_forbidden = true;
  if (play_status != isStopped) play_status = isPaused;
  nul_volume_bars ();
  really_no_status_refresh = true;
  reset_player ();

  memcpy (&ins__fmreg_table_backup, &songdata.instr_macros[test_ins1 - 1], sizeof (ins__fmreg_table_backup));
  memcpy (&ins__dis_fmreg_col_backup, &songdata.dis_fmreg_col[test_ins1 - 1], sizeof (ins__dis_fmreg_col_backup));
  if (test_ins2 != 0)
  {
    memcpy (&ins__fmreg_table_backup2, &songdata.instr_macros[test_ins2 - 1], sizeof (ins__fmreg_table_backup2));
    memcpy (&ins__dis_fmreg_col_backup2, &songdata.dis_fmreg_col[test_ins2 - 1], sizeof (ins__dis_fmreg_col_backup2));
  }
  if ((strcmp (ext, "a2f") == 0) || loadMacros || loadArpVib)
  {
    memcpy (&songdata.instr_macros[test_ins1 - 1], &temp_ins->ins1.macro, sizeof (songdata.instr_macros[0]));
    memcpy (&songdata.dis_fmreg_col[test_ins1 - 1], &temp_ins->ins1.dis_fmreg_col, sizeof (songdata.dis_fmreg_col[0]));
    if (test_ins2 != 0)
    {
      memcpy (&songdata.instr_macros[test_ins2 - 1], &temp_ins->ins2.macro, sizeof (songdata.instr_macros[0]));
      memcpy (&songdata.dis_fmreg_col[test_ins2 - 1], &temp_ins->ins2.dis_fmreg_col, sizeof (songdata.dis_fmreg_col[0]));
    }
  }
  else
  {
    memset (&songdata.instr_macros[test_ins1 - 1], 0, sizeof (songdata.instr_macros[0]));
    memset (&songdata.dis_fmreg_col[test_ins1 - 1], 0, sizeof (songdata.dis_fmreg_col[0]));
    if (test_ins2 != 0)
    {
      memset (&songdata.instr_macros[test_ins2 - 1], 0, sizeof (songdata.instr_macros[0]));
      memset (&songdata.dis_fmreg_col[test_ins2 - 1], 0, sizeof (songdata.dis_fmreg_col[0]));
    }
  }

  memcpy (channel_flag_backup, channel_flag, sizeof (channel_flag_backup));
  memcpy (event_table_backup, event_table, sizeof (event_table_backup));
  common_flag_backup = songdata.common_flag;
  volume_scaling_backup = volume_scaling;
  songdata.common_flag &= ~0x80;
  volume_scaling = false;
  memset (channel_flag, true, sizeof (channel_flag));
  memcpy (pan_lock_backup, pan_lock, sizeof (pan_lock_backup));
  memcpy (volume_lock_backup, volume_lock, sizeof (volume_lock_backup));
  memcpy (peak_lock_backup, peak_lock, sizeof (peak_lock_backup));
  memcpy (panning_table_backup, panning_table, sizeof (panning_table_backup));
  memset (pan_lock, false, sizeof (pan_lock));
  memset (volume_lock, false, sizeof (volume_lock));
  memset (peak_lock, false, sizeof (peak_lock));
  flag_4op_backup = songdata.flag_4op;
  if (songdata.flag_4op == 0)
    channels = percussion_mode ? 15 : 18;
  else
  {
    songdata.flag_4op = test_ins2 != 0 ? 0x3F : 0;
    channels = test_ins2 != 0 ? 6 : (percussion_mode ? 15 : 18);
  }

  reset_player ();
  memset (chan_handle, 0, sizeof (chan_handle));
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
      && (temp_ins->ins1.fm.perc_voice >= 1)
      && (temp_ins->ins1.fm.perc_voice <= 5))
  {
    uint8_t key = board_scancodes[board_pos];
    int note = board_pos + 12 * octave;

    if (note <= 12*8)
    {
      chan_handle[temp_ins->ins1.fm.perc_voice - 1 + 15] = key;
      test_instrument_alt_output_note (temp_ins, temp_ins->ins1.fm.perc_voice - 1 + 15, note, test_ins1 - 1, test_ins2 - 1);
    }

    while (scankey (key))
    {
#if GO32
      realtime_gfx_poll_proc ();
#else // !GO32
      _draw_screen_without_delay = true;
      keyboard_poll_input ();
#endif // !GO32
      keyboard_reset_buffer();
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

        if (!((songdata.flag_4op != 0) && (test_ins2 != 0)))
        {
          chan1 = get_chanpos (chan_handle, channels, key) - 1;
          chan2 = get_chanpos (chan_handle, channels, 0) - 1;
        }
        else
        {
          chan1 = get_chanpos2 (chan_handle, channels, key) - 1;
          chan2 = get_chanpos2 (chan_handle, channels, 0) - 1;
        }

        if (pressed)
        {
          valid_key = true;

          if ((chan1 < 0) && (chan2 >= 0) && (note <= 12*8))
          {
            chan_handle[chan2] = key;
            test_instrument_alt_output_note (temp_ins, chan2, note, test_ins1 - 1, test_ins2 - 1);
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

  memcpy (&songdata.instr_macros[test_ins1 - 1], &ins__fmreg_table_backup, sizeof (songdata.instr_macros[0]));
  memcpy (&songdata.dis_fmreg_col[test_ins1 - 1], &ins__dis_fmreg_col_backup, sizeof (songdata.dis_fmreg_col[0]));
  if (test_ins2 != 0)
  {
    memcpy (&songdata.instr_macros[test_ins2 - 1], &ins__fmreg_table_backup2, sizeof (songdata.instr_macros[0]));
    memcpy (&songdata.dis_fmreg_col[test_ins2 - 1], &ins__dis_fmreg_col_backup2, sizeof (songdata.dis_fmreg_col[0]));
  }

  memcpy (&songdata.macro_table, &arp_vib_macro_table_backup, sizeof (songdata.macro_table));

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
  DBG_LEAVE (); //EXIT //test_instrument_alt
}
