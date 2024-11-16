// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void stop_playing (void)
{
  DBG_ENTER ("stop_playing");

#if !ADT2PLAY
#if !GO32
  flush_WAV_data ();
  if (!opl3_flushmode)
    renew_wav_files_flag = true;
#endif // !GO32
#endif // !ADT2PLAY

  replay_forbidden = true;
  play_status = isStopped;
#if !ADT2PLAY
  fade_out_volume = 63;
  repeat_pattern = false;
#endif // !ADT2PLAY
  global_volume = 63;
#if !ADT2PLAY
  no_sync_playing = false;
  play_single_patt = false;
#endif // !ADT2PLAY
  current_tremolo_depth = tremolo_depth;
  current_vibrato_depth = vibrato_depth;
  pattern_break = false;
  current_order = 0;
  current_pattern = 0;
  current_line = 0;
#if !ADT2PLAY
  start_order = BYTE_NULL;
  start_pattern = BYTE_NULL;
  start_line = BYTE_NULL;
#endif // !ADT2PLAY
  song_timer = 0;
  timer_temp = 0;
  song_timer_tenths = 0;
#if !ADT2PLAY
  time_playing = 0;
#endif // !ADT2PLAY
  playback_speed_shift = 0;

#if ADT2PLAY
  for (uint8_t i = 0; i < 20; i++)
    release_sustaining_sound (i + 1);
#else // !ADT2PLAY
  skip_macro_flag = true;
  for (uint8_t i = 0; i < 20; i++)
    reset_chan_data (i + 1);
#endif // !ADT2PLAY
  opl2out (_instr[11], 0);
#if ADT2PLAY
  opl3exp (0x0004);
  opl3exp (0x0005);
#endif // ADT2PLAY
  init_buffers ();
#if !ADT2PLAY
  skip_macro_flag = false;

  speed = songdata.speed;
  update_timer (songdata.tempo);
#endif // !ADT2PLAY

  DBG_LEAVE (); //EXIT //stop_playing
}
