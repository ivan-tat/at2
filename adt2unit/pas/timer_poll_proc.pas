// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure timer_poll_proc;
begin
  _dbg_enter ({$I %FILE%}, 'timer_poll_proc');

{$IFDEF ADT2PLAY}

  If (timer_200hz_counter < (IRQ_freq+IRQ_freq_shift+playback_speed_shift) DIV 200) then
    Inc(timer_200hz_counter)
  else begin
         timer_200hz_counter := 0;
         timer_200hz_flag := TRUE;
       end;

  If (timer_50hz_counter < (IRQ_freq+IRQ_freq_shift+playback_speed_shift) DIV 50) then
    Inc(timer_50hz_counter)
  else begin
         timer_50hz_counter := 0;
         timer_50hz_flag := TRUE;
       end;

  If (timer_20hz_counter < (IRQ_freq+IRQ_freq_shift+playback_speed_shift) DIV 20) then
    Inc(timer_20hz_counter)
  else begin
         timer_20hz_counter := 0;
         timer_20hz_flag := TRUE;
       end;

  If NOT replay_forbidden then
    begin
      If (current_order = 0) and (current_line = 0) and
         (tick0 = ticks) then
        begin
          song_timer := 0;
          timer_temp := 0;
          song_timer_tenths := 0;
        end;

{$ELSE} // NOT DEFINED(ADT2PLAY)

{$IFDEF GO32V2}

  If (timer_determinator < IRQ_freq+IRQ_freq_shift+playback_speed_shift) then
    Inc(timer_determinator)
  else begin
         timer_determinator := 1;
         Inc(seconds_counter);
       end;

  If (timer_det2 < (IRQ_freq+IRQ_freq_shift+playback_speed_shift) DIV 100) then
    Inc(timer_det2)
  else begin
         timer_det2 := 1;
         Inc(hundereds_counter);
         Inc(_cursor_blink_pending_frames);
         Inc(_screen_refresh_pending_frames);
       end;

{$ENDIF} // DEFINED(GO32V2)

{$IFDEF GO32V2}

  If NOT reset_gfx_ticks then Inc(gfx_ticks)
  else begin
         gfx_ticks := 0;
         reset_gfx_ticks := FALSE;
       end;

  scroll_ticks := scroll_ticks+1/(IRQ_freq+IRQ_freq_shift+playback_speed_shift)*100;
  If (scroll_ticks > 0.2) then
    begin
      If ctrl_tab_pressed then
        begin
          If scankey(SC_UP) then
            If (scr_scroll_y > 0) then
              begin
                Dec(scr_scroll_y);
                update_mouse_position;
              end;

          If scankey(SC_DOWN) then
            If (scr_scroll_y < scr_font_height*MaxLn-scr_font_height*hard_maxln) then
              begin
                Inc(scr_scroll_y);
                update_mouse_position;
              end;

          If scankey(SC_PAGEUP) then
            begin
              scr_scroll_y := 0;
              update_mouse_position;
            end;

          If scankey(SC_PAGEDOWN) then
            begin
              scr_scroll_y := scr_font_height*MaxLn-scr_font_height*hard_maxln;
              update_mouse_position;
            end;

          If scankey(SC_HOME) then
            begin
              scr_scroll_x := 0;
              update_mouse_position;
            end;

          If scankey(SC_END) then
            begin
              scr_scroll_x := scr_font_width*MaxCol-scr_font_width*hard_maxcol;
              update_mouse_position;
            end;

          If scankey(SC_LEFT) then
            If (scr_scroll_x > 0) then
              begin
                Dec(scr_scroll_x);
                update_mouse_position;
              end;

          If scankey(SC_RIGHT) then
            If (scr_scroll_x < scr_font_width*MaxCol-scr_font_width*hard_maxcol) then
              begin
                Inc(scr_scroll_x);
                update_mouse_position;
              end;
        end;
      scroll_ticks := 0;
    end;

{$ELSE} // NOT DEFINED(GO32V2)

  Inc(blink_ticks);
  If ((fast_forward or _rewind or (space_pressed and debugging) or
       (@macro_preview_indic_proc <> NIL)) and
      (blink_ticks > 50)) or (blink_ticks > 50) then
    begin
      blink_flag := NOT blink_flag;
      blink_ticks := 0;
    end;

  Inc(_WAVREC_blink_ticks);
  If ((fast_forward or _rewind or (space_pressed and debugging) or
       (@macro_preview_indic_proc <> NIL)) and
      (_WAVREC_blink_ticks > 50)) or (_WAVREC_blink_ticks > 50) then
    begin
      _WAVREC_blink_flag := NOT _WAVREC_blink_flag;
      _WAVREC_blink_ticks := 0;
    end;

  Inc(_NRECM_blink_ticks);
  If ((fast_forward or _rewind or (space_pressed and debugging) or
       (@macro_preview_indic_proc <> NIL)) and
      (_NRECM_blink_ticks > 50)) or (_NRECM_blink_ticks > 50) then
    begin
      _NRECM_blink_flag := NOT _NRECM_blink_flag;
      _NRECM_blink_ticks := 0;
    end;

  Inc(_IRQFREQ_blink_ticks);
  If ((fast_forward or _rewind or (space_pressed and debugging) or
       (@macro_preview_indic_proc <> NIL)) and
      (_IRQFREQ_blink_ticks > 25)) or (_IRQFREQ_blink_ticks > 25) then
    begin
      _IRQFREQ_blink_flag := NOT _IRQFREQ_blink_flag;
      _IRQFREQ_blink_ticks := 0;
    end;

  If ctrl_tab_pressed then
    begin
      If scankey(SC_UP) then
        If (screen_scroll_offset > 0) then
          Dec(screen_scroll_offset,2);

      If scankey(SC_DOWN) then
        If (screen_scroll_offset < 16*MaxLn-16*hard_maxln) then
          Inc(screen_scroll_offset,2);

      If scankey(SC_PAGEUP) then
        screen_scroll_offset := 0;

      If scankey(SC_PAGEDOWN) then
        screen_scroll_offset := 16*MaxLn-16*hard_maxln;
    end;

{$ENDIF} // NOT DEFINED(GO32V2)
{$ENDIF} // NOT DEFINED(ADT2PLAY)

{$IFDEF ADT2PLAY}
      If (play_status = isPlaying) then
{$ELSE} // NOT DEFINED(ADT2PLAY)
      If (play_status = isPlaying) and
         NOT (debugging and (NOT space_pressed or no_step_debugging)) then
{$ENDIF} // NOT DEFINED(ADT2PLAY)
        begin
{$IFDEF GO32V2}
          song_timer_tenths := Trunc(100/(IRQ_freq+IRQ_freq_shift+playback_speed_shift)*timer_temp);
{$ELSE} // NOT DEFINED(GO32V2)
          song_timer_tenths := timer_temp;
{$ENDIF} // NOT DEFINED(GO32V2)
          If (song_timer_tenths >= 100) then song_timer_tenths := 0;
{$IFDEF GO32V2}
          If (timer_temp < IRQ_freq+IRQ_freq_shift+playback_speed_shift) then Inc(timer_temp)
{$ELSE} // NOT DEFINED(GO32V2)
          If (timer_temp < 100) then Inc(timer_temp)
{$ENDIF} // NOT DEFINED(GO32V2)
          else begin
                 Inc(song_timer);
                 timer_temp := 1;
               end;
{$IFDEF ADT2PLAY}
        end;
{$ELSE} // NOT DEFINED(ADT2PLAY)
        end
      else If debugging and (NOT space_pressed or no_step_debugging) then
             If NOT pattern_delay then synchronize_song_timer;
{$ENDIF} // NOT DEFINED(ADT2PLAY)

      If (song_timer > 3600-1) then
        begin
          song_timer := 0;
          timer_temp := 0;
          song_timer_tenths := 0;
        end;

{$IFDEF GO32V2}
{$IFDEF ADT2PLAY}
      If (ticklooper = 0) or fast_forward then
        poll_proc;
{$ELSE} // NOT DEFINED(ADT2PLAY)
      If (ticklooper > 0) then
        If (fast_forward or _rewind) and NOT replay_forbidden then
          poll_proc
        else
      else If NOT replay_forbidden then
             poll_proc;
{$ENDIF} // NOT DEFINED(ADT2PLAY)

      If (macro_ticklooper = 0) then
        macro_poll_proc;

      Inc(ticklooper);
      If (ticklooper >= IRQ_freq DIV tempo) then
        ticklooper := 0;

      Inc(macro_ticklooper);
      If (macro_ticklooper >= IRQ_freq DIV (tempo*_macro_speedup)) then
        macro_ticklooper := 0;
{$ENDIF} // DEFINED(GO32V2)
{$IFDEF ADT2PLAY}
    end;
{$ENDIF} // DEFINED(ADT2PLAY)

  _dbg_leave; //EXIT //timer_poll_proc
end;
