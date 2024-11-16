// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure stop_playing;

var
  temp: Byte;

begin
  _dbg_enter ({$I %FILE%}, 'stop_playing');

{$IFNDEF ADT2PLAY}
 {$IFNDEF GO32V2}
  flush_WAV_data;
  If NOT opl3_flushmode then
    renew_wav_files_flag := TRUE;
 {$ENDIF} // NOT DEFINED(GO32V2)
{$ENDIF} // NOT DEFINED(ADT2PLAY)

  replay_forbidden := TRUE;
  play_status := isStopped;
{$IFNDEF ADT2PLAY}
  fade_out_volume := 63;
  repeat_pattern := FALSE;
{$ENDIF} // NOT DEFINED(ADT2PLAY)
  global_volume := 63;
{$IFNDEF ADT2PLAY}
  no_sync_playing := FALSE;
  play_single_patt := FALSE;
{$ENDIF} // NOT DEFINED(ADT2PLAY)
  current_tremolo_depth := tremolo_depth;
  current_vibrato_depth := vibrato_depth;
  pattern_break := FALSE;
  current_order := 0;
  current_pattern := 0;
  current_line := 0;
{$IFNDEF ADT2PLAY}
  start_order := BYTE_NULL;
  start_pattern := BYTE_NULL;
  start_line := BYTE_NULL;
{$ENDIF} // NOT DEFINED(ADT2PLAY)
  song_timer := 0;
  timer_temp := 0;
  song_timer_tenths := 0;
{$IFNDEF ADT2PLAY}
  time_playing := 0;
{$ENDIF} // NOT DEFINED(ADT2PLAY)
  playback_speed_shift := 0;

{$IFDEF ADT2PLAY}
  For temp := 1 to 20 do release_sustaining_sound(temp);
{$ELSE} // NOT DEFINED(ADT2PLAY)
  skip_macro_flag := TRUE;
  For temp := 1 to 20 do reset_chan_data(temp);
{$ENDIF} // NOT DEFINED(ADT2PLAY)
  opl2out(_instr[11],0);
{$IFDEF ADT2PLAY}
  opl3exp($0004);
  opl3exp($0005);
{$ENDIF} // DEFINED(ADT2PLAY)
  init_buffers;
{$IFNDEF ADT2PLAY}
  skip_macro_flag := FALSE;

  speed := songdata.speed;
  update_timer(songdata.tempo);
{$ENDIF} // NOT DEFINED(ADT2PLAY)

  _dbg_leave; //EXIT //stop_playing
end;
