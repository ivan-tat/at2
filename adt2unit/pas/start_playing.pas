// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure start_playing;
begin
  _dbg_enter ({$I %FILE%}, 'start_playing');

{$IFDEF ADT2PLAY}
  stop_playing;
  If (error_code <> 0) then
    begin
      _dbg_leave; EXIT; //start_playing
    end;
{$ENDIF} // DEFINED(ADT2PLAY)

  init_player;

{$IFDEF ADT2PLAY}
  current_order := 0;
  If (songdata.pattern_order[current_order] > $7f) then
    If (calc_order_jump = -1) then
      begin
        _dbg_leave; EXIT; //start_playing
      end;

  current_pattern := songdata.pattern_order[current_order];
  current_line := 0;
{$ELSE} // NOT DEFINED(ADT2PLAY)
  If (start_pattern = BYTE_NULL) then current_order := 0
  else If (start_order = BYTE_NULL) then
         begin
           If (calc_pattern_pos(start_pattern) <> BYTE_NULL) then
             current_order := calc_pattern_pos(start_pattern)
           else If NOT play_single_patt then
                  begin
                    start_pattern := BYTE_NULL;
                    current_order := 0;
                    _dbg_leave; EXIT; //start_playing
                  end;
         end
       else begin
              current_order := start_order;
              current_pattern := start_pattern;
            end;

  If NOT play_single_patt then
    If (songdata.pattern_order[current_order] > $7f) then
      If (calc_order_jump = -1) then
        begin
          _dbg_leave; EXIT; //start_playing
        end;

  If NOT play_single_patt then
    current_pattern := songdata.pattern_order[current_order]
  else current_pattern := start_pattern;

  If (start_line = BYTE_NULL) then current_line := 0
  else current_line := start_line;
{$ENDIF} // NOT DEFINED(ADT2PLAY)

  pattern_break := FALSE;
  pattern_delay := FALSE;
  tickXF := 0;
  ticks := 0;
  tick0 := 0;
{$IFNDEF ADT2PLAY}
  last_order := 0;
{$ENDIF} // NOT DEFINED(ADT2PLAY)
  next_line := 0;
  song_timer := 0;
  timer_temp := 0;
  song_timer_tenths := 0;
  time_playing := 0;
  ticklooper := 0;
  macro_ticklooper := 0;
{$IFNDEF ADT2PLAY}
  debugging := FALSE;
  fade_out_volume := 63;
{$ENDIF} // NOT DEFINED(ADT2PLAY)
  playback_speed_shift := 0;
  replay_forbidden := FALSE;
  play_status := isPlaying;
  speed := songdata.speed;
  macro_speedup := songdata.macro_speedup;
{$IFDEF ADT2PLAY}
  IRQ_freq_shift := songdata.bpm_data.tempo_finetune;
{$ENDIF} // DEFINED(ADT2PLAY)

  update_timer(songdata.tempo);

{$IFNDEF ADT2PLAY}
  no_status_refresh := FALSE;
  really_no_status_refresh := FALSE;
  FillChar(play_pos_buf,SizeOf(play_pos_buf),0);
{$ENDIF} // NOT DEFINED(ADT2PLAY)

  _dbg_leave; //EXIT //start_playing
end;
