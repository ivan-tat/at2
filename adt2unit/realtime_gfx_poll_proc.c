// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void realtime_gfx_poll_proc (void)
{
  if (_realtime_gfx_no_update)
    return; //realtime_gfx_poll_proc

#if GO32

  if ((!reset_gfx_ticks)
      && (gfx_ticks > ((IRQ_freq + IRQ_freq_shift + playback_speed_shift) / 100) * (fps_down_factor + 1)))
  {
    blink_ticks++;
    if (blink_ticks == 40)
    {
      blink_flag = !blink_flag;
      blink_ticks = 0;
    }

    _NRECM_blink_ticks++;
    if (_NRECM_blink_ticks == 40)
    {
      _NRECM_blink_flag = !_NRECM_blink_flag;
      _NRECM_blink_ticks = 0;
    }

    _IRQFREQ_blink_ticks++;
    if (_IRQFREQ_blink_ticks == 20)
    {
      _IRQFREQ_blink_flag = !_IRQFREQ_blink_flag;
      _IRQFREQ_blink_ticks = 0;
    }

#endif // GO32

    if (blink_flag)
    {
      status_layout[isPlaying][9] = (!debugging) ? '\x10' : '\x09';
      status_layout[isPaused] [8] = '\x08';
      status_layout[isStopped][9] = (debugging && (play_status == isStopped)) ? '\x09' : ' ';
      if (macro_preview_indic_proc)
        macro_preview_indic_proc (1);
    }
    else
    {
      status_layout[isPlaying][9] = ' ';
      status_layout[isPaused] [8] = ' ';
      status_layout[isStopped][9] = ' ';
      if (macro_preview_indic_proc)
        macro_preview_indic_proc (2);
    }

    if (_show_bpm_realtime_proc)
      _show_bpm_realtime_proc ();

    decay_bars_refresh ();

#if !GO32
    if (opl3_channel_recording_mode)
      update_recorded_channels ();
#endif // !GO32

    if (do_synchronize)
      synchronize_screen ();

#if !GO32
    if (_name_scrl_pending_frames > 0)
      _name_scrl_pending_frames--;

    _cursor_blink_pending_frames++;
#endif // !GO32

    status_refresh ();
    STATUS_LINE_refresh ();

#if GO32
    reset_gfx_ticks = true;
#endif // GO32

    trace_update_proc ();

    if (mn_environment.ext_proc_rt)
      mn_environment.ext_proc_rt ();
#if GO32
  }
#endif // GO32

  //EXIT //realtime_gfx_poll_proc
}
