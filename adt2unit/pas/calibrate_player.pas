// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure calibrate_player(order,line: Byte; status_filter: Boolean;
                           line_dependent: Boolean);

var
  temp_channel_flag: array[1..20] of Boolean;
  old_debugging,
  old_repeat_pattern: Boolean;
  jump_count,loop_count,
  temp,previous_order,previous_line: Byte;
  status_backup: Record
                   replay_forbidden: Boolean;
                   play_status: tPLAY_STATUS;
                 end;

var
  _pattern_page,_pattord_page,
  _pattord_hpos,_pattord_vpos: Byte;

begin
  _dbg_enter ({$I %FILE%}, 'calibrate_player');

  If (calc_following_order(0) = -1) then
    begin
      _dbg_leave; EXIT; //calibrate_player
    end;
  calibrating := TRUE;
  status_backup.replay_forbidden := replay_forbidden;
  status_backup.play_status := play_status;
  If status_filter then no_status_refresh := TRUE;

  nul_volume_bars;
  Move(channel_flag,temp_channel_flag,SizeOf(temp_channel_flag));
  FillChar(channel_flag,SizeOf(channel_flag),BYTE(FALSE));

  old_debugging := debugging;
  old_repeat_pattern := repeat_pattern;
  debugging := FALSE;
  repeat_pattern := FALSE;

  If (play_status = isStopped) or
     (order < current_order) or
     (order = calc_following_order(0)) then
    begin
      If NOT no_sync_playing then stop_playing
      else begin
             stop_playing;
             no_sync_playing := TRUE;
           end;

      init_player;
      speed := songdata.speed;
      macro_speedup := songdata.macro_speedup;
      update_timer(songdata.tempo);
      current_order := calc_following_order(0);
      current_pattern := songdata.pattern_order[current_order];
      current_line := 0;
      pattern_break := FALSE;
      pattern_delay := FALSE;
      last_order := 0;
      next_line := 0;
      song_timer := 0;
      timer_temp := 0;
      song_timer_tenths := 0;
      time_playing := 0;
      ticklooper := 0;
      macro_ticklooper := 0;
      ticks := 0;
      tick0 := 0;
    end;

  If NOT no_sync_playing then
    begin
      show_str(13,07,' --:--.- ',status_background+status_border);
{$IFNDEF GO32V2}
      _draw_screen_without_delay := TRUE;
{$ENDIF}
      draw_screen;
    end;

  previous_order := current_order;
  previous_line := current_line;
  jump_count := 0;
  loop_count := 0;
  replay_forbidden := TRUE;
  seek_pattern_break := FALSE;

  If NOT no_sync_playing then
    While (current_line <> line) or
          (current_order <> order) do
      begin
        If scankey(SC_ESCAPE) then BREAK;
        If NOT ((previous_order = current_order) and
                (previous_line >= current_line) and NOT (pattern_break and
                (next_line AND $0f0 = pattern_loop_flag))) then loop_count := 0
        else begin
               Inc(loop_count);
               If (loop_count > 15) then BREAK;
             end;

        If (current_order = order) and (current_line >= line) and
           NOT line_dependent then BREAK;

        previous_order := current_order;
        previous_line := current_line;
        fast_forward := TRUE;

        poll_proc;
        If (macro_ticklooper = 0) then
          macro_poll_proc;

        fast_forward := FALSE;
        Inc(ticklooper);
        If (ticklooper >= IRQ_freq DIV tempo) then
          ticklooper := 0;

        Inc(macro_ticklooper);
        If (macro_ticklooper >= IRQ_freq DIV (tempo*macro_speedup)) then
          macro_ticklooper := 0;

        If (previous_order <> current_order) then
          begin
            update_status;
            Inc(jump_count);
            If (jump_count > $7f) then BREAK;
          end;

        If seek_pattern_break then
          If (current_order > order) or
             ((current_order = order) and
              ((current_line <> 0) or (line = 0))) then BREAK
          else seek_pattern_break := FALSE;
        keyboard_reset_buffer;
      end
  else
    begin
      start_playing;
      current_order := order;
      current_pattern := songdata.pattern_order[order];
      current_line := line;
    end;

  fade_out_volume := 63;
  Move(temp_channel_flag,channel_flag,SizeOf(channel_flag));

  For temp := 1 to 20 do reset_chan_data(temp);
  If (status_backup.play_status <> isStopped) then
    begin
      replay_forbidden := status_backup.replay_forbidden;
      play_status := status_backup.play_status;
    end
  else begin
         replay_forbidden := FALSE;
         play_status := isPlaying;
       end;

  debugging := old_debugging;
  repeat_pattern := old_repeat_pattern;
  synchronize_song_timer;
  calibrating := FALSE;
  If status_filter then no_status_refresh := FALSE;

  _pattern_page := line;
  _pattord_page := 0;
  _pattord_hpos := 1;
  _pattord_vpos := 1;

  While (current_order <> _pattord_vpos+4*(_pattord_hpos+_pattord_page-1)-1) do
    If (_pattord_vpos < 4) then Inc(_pattord_vpos)
    else If (_pattord_hpos < MAX_ORDER_COLS) then begin Inc(_pattord_hpos); _pattord_vpos := 1; end
         else If (_pattord_page < 23-(MAX_ORDER_COLS-9)) then begin Inc(_pattord_page); _pattord_vpos := 1; end;

  If tracing then
    begin
      PATTERN_ORDER_page_refresh(_pattord_page);
      PATTERN_page_refresh(_pattern_page);
    end;

  keyboard_reset_buffer;

  _dbg_leave; //EXIT //calibrate_player
end;
