// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void update_song_position (void)
{
  DBG_ENTER ("update_song_position");

#if !ADT2PLAY
#if USE_FPC
  // HINT: GCC optimizes this with `memmove' from GLIBC but not with `custom_memmove'
  memmove (&play_pos_buf[1], play_pos_buf, sizeof (play_pos_buf) - sizeof (play_pos_buf[0]));
#else // !USE_FPC
  for (uint8_t i = 8; i; i--)
    play_pos_buf[i] = play_pos_buf[i - 1];
#endif // !USE_FPC
  play_pos_buf[0] = (current_pattern << 8) + current_line;

  if (!_rewind)
  {
#endif // !ADT2PLAY
    if ((current_line < (songdata.patt_len - 1)) && !pattern_break)
      current_line++;
    else
    {
#if !ADT2PLAY
      if ((!(pattern_break && ((next_line & 0xF0) == pattern_loop_flag)))
          && repeat_pattern)
      {
        memset (loopbck_table, BYTE_NULL, sizeof (loopbck_table));
        memset (loop_table, BYTE_NULL, sizeof (loop_table));
        current_line = 0;
        pattern_break = false;
      }
      else
      {
#endif // !ADT2PLAY
        if ((!(pattern_break && ((next_line & 0xF0) == pattern_loop_flag)))
            && ((current_order + 1) < 0x80))
        {
          memset (loopbck_table, BYTE_NULL, sizeof (loopbck_table));
          memset (loop_table, BYTE_NULL, sizeof (loop_table));
          current_order++;
        }

        if (pattern_break && ((next_line & 0xF0) == pattern_loop_flag))
        {
          uint8_t i = next_line - pattern_loop_flag - 1;
          next_line = loopbck_table[i];
          if (loop_table[i][current_line])
            loop_table[i][current_line]--;
        }
        else
        {
          if (pattern_break && ((next_line & 0xF0) == pattern_break_flag))
          {
            const tCHUNK *et = &event_table[next_line - pattern_break_flag - 1];
            current_order = (et->effect_def2 == ef_PositionJump) ? et->effect2
                                                                 : et->effect;
            pattern_break = false;
          }
          else
            if (current_order >= 0x80)
              current_order = 0;
        }

#if !ADT2PLAY
        if (!play_single_patt)
#endif // !ADT2PLAY
          if (songdata.pattern_order[current_order] >= 0x80)
            if (calc_order_jump () == -1)
            {
              DBG_LEAVE (); //update_song_position
              return;
            }

#if !ADT2PLAY
        if (!play_single_patt)
#endif // !ADT2PLAY
          current_pattern = songdata.pattern_order[current_order];

        if (!pattern_break)
          current_line = 0;
        else
        {
          pattern_break = false;
          current_line = next_line;
        }
      }
#if !ADT2PLAY
    }
  }
  else
    if (current_line > 0)
      current_line--;
#endif // !ADT2PLAY

  for (uint8_t i = 0; i < songdata.nm_tracks; i++)
  {
#if !ADT2PLAY
    ignore_note_once[i] = false;
#endif // !ADT2PLAY
    glfsld_table[i] = 0;
    glfsld_table2[i] = 0;
  }

#if !ADT2PLAY
  if (!play_single_patt)
#endif // !ADT2PLAY
    if (   (!current_line)
        && (current_order == calc_following_order (0))
        && speed_update)
    {
      tempo = songdata.tempo;
      speed = songdata.speed;
      update_timer (tempo);
    }

  DBG_LEAVE (); //EXIT //update_song_position
}
