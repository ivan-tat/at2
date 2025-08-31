// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void test_instrument_alt (uint8_t chan, AT2ExtKeyCode fkey,
                          bool loadMacros,
                          bool bankSelector,
                          bool loadArpVib,
                          uint8_t test_ins1, uint8_t test_ins2)
{
  uint8_t tone;
  String_t fname, ext;
  uint8_t channels;

  DBG_ENTER ("test_instrument_alt");

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
  memset (&temp_instrument, 0, sizeof (temp_instrument));
  memset (&temp_instrument2, 0, sizeof (temp_instrument2));

  {
    String_t t;

    CopyString ((String *)&fname, mn_environment.curr_item, 255);
    t = ExtOnly ((String *)&fname);
    ext = Lower ((String *)&t);
  }

  load_flag_alt = UINT8_NULL;

  if (bankSelector || loadArpVib)
  {
    if (!loadArpVib)
    {
      CopyString ((String *)&fname, (String *)"\x00" "",  255);
      memcpy (&temp_instrument,
              &temp_songdata.instr_data[test_ins1],
              sizeof (temp_instrument));
      memcpy (&temp_instrument_macro,
              &temp_songdata.instr_macros[test_ins1],
              sizeof (temp_instrument_macro));
      memcpy (&temp_instrument_dis_fmreg_col,
              &temp_songdata.dis_fmreg_col[test_ins1],
              sizeof (temp_instrument_dis_fmreg_col));
      if (test_ins2 != 0)
      {
        memcpy (&temp_instrument2,
                &temp_songdata.instr_data[test_ins2],
                sizeof (temp_instrument2));
        memcpy (&temp_instrument_macro2,
                &temp_songdata.instr_macros[test_ins2],
                sizeof (temp_instrument_macro2));
        memcpy (&temp_instrument_dis_fmreg_col2,
                &temp_songdata.dis_fmreg_col[test_ins2],
                sizeof (temp_instrument_dis_fmreg_col2));
      }
    }
    else
    {
      memcpy (&temp_instrument,
              &songdata.instr_data[test_ins1],
              sizeof (temp_instrument));
      memcpy (&temp_instrument_macro,
              &songdata.instr_macros[test_ins1],
              sizeof (temp_instrument_macro));
      memcpy (&temp_instrument_dis_fmreg_col,
              &songdata.dis_fmreg_col[test_ins1],
              sizeof (temp_instrument_dis_fmreg_col));
      if (test_ins2 != 0)
      {
        memcpy (&temp_instrument2,
                &songdata.instr_data[test_ins2],
                sizeof (temp_instrument2));
        memcpy (&temp_instrument_macro2,
                &songdata.instr_macros[test_ins2],
                sizeof (temp_instrument_macro2));
        memcpy (&temp_instrument_dis_fmreg_col2,
                &songdata.dis_fmreg_col[test_ins2],
                sizeof (temp_instrument_dis_fmreg_col2));
      }
    }
  }
  else
  {
#if GO32
    keyboard_toggle_sleep ();
#endif // GO32

    // Loader may change `load_flag_alt'
    if      (memcmp (&ext, (String *)"\x03" "a2i", 4) == 0) a2i_file_loader_alt ((String *)&fname);
    else if (memcmp (&ext, (String *)"\x03" "a2f", 4) == 0) a2f_file_loader_alt ((String *)&fname);
    else if (memcmp (&ext, (String *)"\x03" "cif", 4) == 0) cif_file_loader_alt ((String *)&fname);
    else if (memcmp (&ext, (String *)"\x03" "fin", 4) == 0) fin_file_loader_alt ((String *)&fname);
    else if (memcmp (&ext, (String *)"\x03" "ins", 4) == 0) ins_file_loader_alt ((String *)&fname);
    else if (memcmp (&ext, (String *)"\x03" "sbi", 4) == 0) sbi_file_loader_alt ((String *)&fname);
    else if (memcmp (&ext, (String *)"\x03" "sgi", 4) == 0) sgi_file_loader_alt ((String *)&fname);

#if GO32
    keyboard_toggle_sleep ();
#endif // GO32
    if (load_flag_alt == UINT8_NULL) goto _exit;
  }

  if (   (test_ins2 != 0)
      && Empty (&temp_instrument2, sizeof (temp_instrument2))
      && Empty (&temp_instrument_macro2, sizeof (temp_instrument_macro2)))
    test_ins2 = 0;

  memcpy (&arp_vib_macro_table_backup,
          &songdata.macro_table,
          sizeof (arp_vib_macro_table_backup));

  if (loadArpVib)
  {
    if (arp_tab_selected)
    {
      temp_instrument_macro.arpeggio_table = arpvib_arpeggio_table;
      memcpy (&songdata.macro_table[arpvib_arpeggio_table].arpeggio,
              &temp_songdata.macro_table[arpvib_arpeggio_table].arpeggio,
              sizeof (songdata.macro_table[0].arpeggio)); // HINT: was missing `.arpeggio'
    }
    else
      temp_instrument_macro.arpeggio_table = 0;

    if (vib_tab_selected)
    {
      temp_instrument_macro.vibrato_table = arpvib_vibrato_table;
      memcpy (&songdata.macro_table[arpvib_vibrato_table].vibrato,
              &temp_songdata.macro_table[arpvib_vibrato_table].vibrato,
              sizeof (songdata.macro_table[0].vibrato)); // HINT: was missing `.vibrato'
    }
    else
      temp_instrument_macro.vibrato_table = 0;

    memcpy (&songdata.instr_macros[test_ins1],
            &temp_instrument_macro,
            sizeof (songdata.instr_macros[0]));
    if (test_ins2 != 0)
      memcpy (&songdata.instr_macros[test_ins2],
              &temp_instrument_macro2,
              sizeof (songdata.instr_macros[0]));
  }
  else if (bankSelector && loadMacros)
  {
    if (temp_instrument_macro.arpeggio_table != 0)
      memcpy (&songdata.macro_table[temp_instrument_macro.arpeggio_table].arpeggio,
              &temp_songdata.macro_table[temp_instrument_macro.arpeggio_table].arpeggio,
              sizeof (songdata.macro_table[0].arpeggio)); // HINT: was missing `.arpeggio'
    if (temp_instrument_macro.vibrato_table != 0)
      memcpy (&songdata.macro_table[temp_instrument_macro.vibrato_table].vibrato,
              &temp_songdata.macro_table[temp_instrument_macro.vibrato_table].vibrato,
              sizeof (songdata.macro_table[0].vibrato)); // HINT: was missing `.vibrato'
  }

  if (   Empty (&temp_instrument, sizeof (temp_instrument))
      && Empty (&temp_instrument_macro, sizeof (temp_instrument_macro))
      || (   (test_ins2 != 0)
          && Empty (&temp_instrument2, sizeof (temp_instrument2))
          && Empty (&temp_instrument_macro2, sizeof (temp_instrument_macro2))))
    goto _exit;

  ins_trailing_flag = true;
  status_backup.replay_forbidden = replay_forbidden;
  status_backup.play_status = play_status;
  replay_forbidden = true;
  if (play_status != isStopped) play_status = isPaused;
  nul_volume_bars ();
  really_no_status_refresh = true;
  reset_player ();

  if ((memcmp (&ext, (String *)"\x03" "a2f", 4) == 0) || loadMacros || loadArpVib)
  {
    memcpy (&ins__fmreg_table_backup,
            &songdata.instr_macros[test_ins1],
            sizeof (ins__fmreg_table_backup));
    memcpy (&ins__dis_fmreg_col_backup,
            &songdata.dis_fmreg_col[test_ins1],
            sizeof (ins__dis_fmreg_col_backup));
    memcpy (&songdata.instr_macros[test_ins1],
            &temp_instrument_macro,
            sizeof (songdata.instr_macros[0]));
    memcpy (&songdata.dis_fmreg_col[test_ins1],
            &temp_instrument_dis_fmreg_col,
            sizeof (songdata.dis_fmreg_col[0]));
    if (test_ins2 != 0)
    {
      memcpy (&ins__fmreg_table_backup2,
              &songdata.instr_macros[test_ins2],
              sizeof (ins__fmreg_table_backup));
      memcpy (&ins__dis_fmreg_col_backup2,
              &songdata.dis_fmreg_col[test_ins2],
              sizeof (ins__dis_fmreg_col_backup));
      memcpy (&songdata.instr_macros[test_ins2],
              &temp_instrument_macro2,
              sizeof (songdata.instr_macros[0]));
      memcpy (&songdata.dis_fmreg_col[test_ins2],
              &temp_instrument_dis_fmreg_col2,
              sizeof (songdata.dis_fmreg_col[0]));
    }
  }
  else
  {
    memcpy (&ins__fmreg_table_backup,
            &songdata.instr_macros[test_ins1],
            sizeof (ins__fmreg_table_backup));
    memcpy (&ins__dis_fmreg_col_backup,
            &songdata.dis_fmreg_col[test_ins1],
            sizeof (ins__dis_fmreg_col_backup));
    memset (&songdata.instr_macros[test_ins1], 0, sizeof (songdata.instr_macros[0]));
    memset (&songdata.dis_fmreg_col[test_ins1], 0, sizeof (songdata.dis_fmreg_col[0]));

    if (test_ins2 != 0)
    {
      memcpy (&ins__fmreg_table_backup2,
              &songdata.instr_macros[test_ins2],
              sizeof (ins__fmreg_table_backup));
      memcpy (&ins__dis_fmreg_col_backup2,
              &songdata.dis_fmreg_col[test_ins2],
              sizeof (ins__dis_fmreg_col_backup));
      memset (&songdata.instr_macros[test_ins2], 0, sizeof (songdata.instr_macros[0]));
      memset (&songdata.dis_fmreg_col[test_ins2], 0, sizeof (songdata.dis_fmreg_col[0]));
    }
  }

  memcpy (channel_flag_backup, channel_flag, sizeof (channel_flag_backup));
  memcpy (event_table_backup, event_table, sizeof (event_table_backup));
  common_flag_backup = songdata.common_flag;
  volume_scaling_backup = volume_scaling;
  songdata.common_flag &= ~0x80;
  volume_scaling = false;
  memset (channel_flag, true, sizeof (channel_flag));
  memcpy (pan_lock_backup, pan_lock, sizeof (pan_lock));
  memcpy (volume_lock_backup, volume_lock, sizeof (volume_lock));
  memcpy (peak_lock_backup, peak_lock, sizeof (volume_lock));
  memcpy (panning_table_backup, panning_table, sizeof (panning_table));
  memset (pan_lock, 0, sizeof (pan_lock));
  memset (volume_lock, 0, sizeof (volume_lock));
  memset (peak_lock, 0, sizeof (volume_lock));
  flag_4op_backup = songdata.flag_4op;
  if (!percussion_mode && (songdata.flag_4op == 0))
    channels = 18;
  else if (songdata.flag_4op == 0)
    channels = 15;
  else if (test_ins2 != 0)
  {
    songdata.flag_4op = 0x3F;
    channels = 6;
  }
  else
  {
    songdata.flag_4op = 0;
    channels = percussion_mode ? 15 : 18;
  }

  reset_player ();
  memset (chan_handle, 0, sizeof (chan_handle));
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
    test_instrument_alt_output_note (temp_instrument.perc_voice + 15, tone + 1, test_ins1, test_ins2);
    while (scankey (board_scancodes[tone]))
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
      valid_key = false;
      for (uint8_t i = 0; i < 29; i++)
      {
        uint8_t key = board_scancodes[i];
        bool pressed = scankey (key);
        uint8_t pos1, pos2;

        if (!((songdata.flag_4op != 0) && (test_ins2 != 0)))
        {
          pos1 = get_chanpos (chan_handle, channels, key);
          pos2 = get_chanpos (chan_handle, channels, 0);
        }
        else
        {
          pos1 = get_chanpos2 (chan_handle, channels, key);
          pos2 = get_chanpos2 (chan_handle, channels, 0);
        }

        if (pressed)
        {
          valid_key = true;
          if ((pos1 == 0) && (pos2 != 0))
            test_instrument_alt_output_note (pos2, i + 1, test_ins1, test_ins2);
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

  for (uint8_t i = 0; i < 20; i++) key_off (i + 1);

  memcpy (&songdata.instr_macros[test_ins1],
          &ins__fmreg_table_backup,
          sizeof (songdata.instr_macros[0]));
  memcpy (&songdata.dis_fmreg_col[test_ins1],
          &ins__dis_fmreg_col_backup,
          sizeof (songdata.dis_fmreg_col[0]));

  if (test_ins2 != 0)
  {
    memcpy (&songdata.instr_macros[test_ins1],
            &ins__fmreg_table_backup,
            sizeof (songdata.instr_macros[0]));
    memcpy (&songdata.dis_fmreg_col[test_ins1],
            &ins__dis_fmreg_col_backup,
            sizeof (songdata.dis_fmreg_col[0]));
    memcpy (&songdata.instr_macros[test_ins2],
            &ins__fmreg_table_backup2,
            sizeof (songdata.instr_macros[0]));
    memcpy (&songdata.dis_fmreg_col[test_ins2],
            &ins__dis_fmreg_col_backup2,
            sizeof (songdata.dis_fmreg_col[0]));
  }

  memcpy (&songdata.macro_table,
          &arp_vib_macro_table_backup,
          sizeof (songdata.macro_table));

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
  DBG_LEAVE (); //EXIT //test_instrument_alt
}
