// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure realtime_gfx_poll_proc;
begin
  If _realtime_gfx_no_update then
    EXIT; //realtime_gfx_poll_proc

{$IFDEF GO32V2}

  If NOT reset_gfx_ticks and
     (gfx_ticks > ((IRQ_freq+IRQ_freq_shift+playback_speed_shift) DIV 100)*SUCC(fps_down_factor)) then
    begin
      Inc(blink_ticks);
      If (blink_ticks = 40) then
        begin
          blink_flag := NOT blink_flag;
          blink_ticks := 0;
        end;

      Inc(_NRECM_blink_ticks);
      If (_NRECM_blink_ticks = 40) then
        begin
          _NRECM_blink_flag := NOT _NRECM_blink_flag;
          _NRECM_blink_ticks := 0;
        end;

      Inc(_IRQFREQ_blink_ticks);
      If (_IRQFREQ_blink_ticks = 20) then
        begin
          _IRQFREQ_blink_flag := NOT _IRQFREQ_blink_flag;
          _IRQFREQ_blink_ticks := 0;
        end;

{$ENDIF}

      If blink_flag then
        begin
          If debugging and (play_status = isStopped) then status_layout[isStopped][9] := #9
          else status_layout[isStopped][9] := ' ';
          If NOT debugging then status_layout[isPlaying][9] := #16
          else status_layout[isPlaying][9] := #9;
          status_layout[isPaused][8] := #8;
          If (@macro_preview_indic_proc <> NIL) then
            macro_preview_indic_proc(1);
        end
      else
        begin
          status_layout[isPlaying][9] := ' ';
          status_layout[isPaused] [8] := ' ';
          status_layout[isStopped][9] := ' ';
          If (@macro_preview_indic_proc <> NIL) then
            macro_preview_indic_proc(2);
        end;

      If (Addr(_show_bpm_realtime_proc) <> NIL) then
        _show_bpm_realtime_proc;

      decay_bars_refresh;
{$IFNDEF GO32V2}
      If opl3_channel_recording_mode then update_recorded_channels;
{$ENDIF}
      If do_synchronize then synchronize_screen;
{$IFNDEF GO32V2}
      If (_name_scrl_pending_frames > 0) then Dec(_name_scrl_pending_frames);
      Inc(_cursor_blink_pending_frames);
{$ENDIF}
      status_refresh;
      STATUS_LINE_refresh;
{$IFDEF GO32V2}
      reset_gfx_ticks := TRUE;
{$ENDIF}
      trace_update_proc;
      If (@mn_environment.ext_proc_rt <> NIL) then
        mn_environment.ext_proc_rt;
{$IFDEF GO32V2}
    end;
{$ENDIF}

  //EXIT //realtime_gfx_poll_proc
end;
