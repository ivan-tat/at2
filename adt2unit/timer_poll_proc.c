// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void timer_poll_proc (void)
{
  DBG_ENTER ("timer_poll_proc");

#if ADT2PLAY

  if (timer_200hz_counter < (IRQ_freq + IRQ_freq_shift + playback_speed_shift) / 200)
    timer_200hz_counter++;
  else
  {
    timer_200hz_counter = 0;
    timer_200hz_flag = true;
  }

  if (timer_50hz_counter < (IRQ_freq + IRQ_freq_shift + playback_speed_shift) / 50)
    timer_50hz_counter++;
  else
  {
    timer_50hz_counter = 0;
    timer_50hz_flag = true;
  }

  if (timer_20hz_counter < (IRQ_freq + IRQ_freq_shift + playback_speed_shift) / 20)
    timer_20hz_counter++;
  else
  {
    timer_20hz_counter = 0;
    timer_20hz_flag = true;
  }

  if (!replay_forbidden)
  {
    if ((current_order == 0) && (current_line == 0) && (tick0 == ticks))
    {
      song_timer = 0;
      timer_temp = 0;
      song_timer_tenths = 0;
    }

#else // !ADT2PLAY

#if GO32

  if (timer_determinator < (IRQ_freq + IRQ_freq_shift + playback_speed_shift))
    timer_determinator++;
  else
  {
    timer_determinator = 1;
    seconds_counter++;
  }

  if (timer_det2 < (IRQ_freq + IRQ_freq_shift + playback_speed_shift) / 100)
    timer_det2++;
  else
  {
    timer_det2 = 1;
    hundereds_counter++;
    _cursor_blink_pending_frames++;
    _screen_refresh_pending_frames++;
  }

  if (!reset_gfx_ticks)
    gfx_ticks++;
  else
  {
    gfx_ticks = 0;
    reset_gfx_ticks = false;
  }

  scroll_ticks += (float) 100 / (IRQ_freq + IRQ_freq_shift + playback_speed_shift);
  if (scroll_ticks > 0.2)
  {
    if (ctrl_tab_pressed ())
    {
      if (scankey (SC_UP))
        if (scr_scroll_y > 0)
        {
          scr_scroll_y--;
          update_mouse_position ();
        }

      if (scankey (SC_DOWN))
        if (scr_scroll_y < (scr_font_height * MaxLn - scr_font_height * hard_maxln))
        {
          scr_scroll_y++;
          update_mouse_position ();
        }

      if (scankey (SC_PAGEUP))
      {
        scr_scroll_y = 0;
        update_mouse_position ();
      }

      if (scankey (SC_PAGEDOWN))
      {
        scr_scroll_y = scr_font_height * MaxLn - scr_font_height * hard_maxln;
        update_mouse_position ();
      }

      if (scankey (SC_HOME))
      {
        scr_scroll_x = 0;
        update_mouse_position ();
      }

      if (scankey (SC_END))
      {
        scr_scroll_x = scr_font_width * MaxCol - scr_font_width * hard_maxcol;
        update_mouse_position ();
      }

      if (scankey (SC_LEFT))
        if (scr_scroll_x > 0)
        {
          scr_scroll_x--;
          update_mouse_position ();
        }

      if (scankey (SC_RIGHT))
        if (scr_scroll_x < (scr_font_width * MaxCol - scr_font_width * hard_maxcol))
        {
          scr_scroll_x++;
          update_mouse_position ();
        }
    }
    scroll_ticks = 0;
  }

#else // !GO32

  blink_ticks++;
  if (((fast_forward || _rewind || (space_pressed && debugging) ||
       (macro_preview_indic_proc != NULL)) &&
      (blink_ticks > 50)) || (blink_ticks > 50))
  {
    blink_flag = !blink_flag;
    blink_ticks = 0;
  }

  _WAVREC_blink_ticks++;
  if (((fast_forward || _rewind || (space_pressed && debugging) ||
       (macro_preview_indic_proc != NULL)) &&
      (_WAVREC_blink_ticks > 50)) || (_WAVREC_blink_ticks > 50))
  {
    _WAVREC_blink_flag = !_WAVREC_blink_flag;
    _WAVREC_blink_ticks = 0;
  }

  _NRECM_blink_ticks++;
  if (((fast_forward || _rewind || (space_pressed && debugging) ||
       (macro_preview_indic_proc != NULL)) &&
      (_NRECM_blink_ticks > 50)) || (_NRECM_blink_ticks > 50))
  {
    _NRECM_blink_flag = !_NRECM_blink_flag;
    _NRECM_blink_ticks = 0;
  }

  _IRQFREQ_blink_ticks++;
  if (((fast_forward || _rewind || (space_pressed && debugging) ||
       (macro_preview_indic_proc != NULL)) &&
      (_IRQFREQ_blink_ticks > 25)) || (_IRQFREQ_blink_ticks > 25))
  {
    _IRQFREQ_blink_flag = !_IRQFREQ_blink_flag;
    _IRQFREQ_blink_ticks = 0;
  }

  if (ctrl_tab_pressed ())
  {
    if (scankey (SC_UP))
      if (screen_scroll_offset > 0)
        screen_scroll_offset -= 2;

    if (scankey (SC_DOWN))
      if (screen_scroll_offset < (16 * MaxLn - 16 * hard_maxln))
        screen_scroll_offset += 2;

    if (scankey (SC_PAGEUP))
      screen_scroll_offset = 0;

    if (scankey (SC_PAGEDOWN))
      screen_scroll_offset = 16 * MaxLn - 16 * hard_maxln;
  }

#endif // !GO32
#endif // !ADT2PLAY

#if ADT2PLAY
      if (play_status == isPlaying)
#else // !ADT2PLAY
      if ((play_status == isPlaying)
          && !(debugging && ((!space_pressed) || no_step_debugging)))
#endif // !ADT2PLAY
      {
#if GO32
        song_timer_tenths = trunc ((double) timer_temp * 100 / (IRQ_freq + IRQ_freq_shift + playback_speed_shift));
#else // !GO32
        song_timer_tenths = timer_temp;
#endif // !GO32
        if (song_timer_tenths >= 100)
          song_timer_tenths = 0;
#if GO32
        if (timer_temp < (IRQ_freq + IRQ_freq_shift + playback_speed_shift))
#else // !GO32
        if (timer_temp < 100)
#endif // !GO32
          timer_temp++;
        else
        {
          song_timer++;
          timer_temp = 1;
        }
      }
#if !ADT2PLAY
      else
        if (debugging && ((!space_pressed) || no_step_debugging))
          if (!pattern_delay)
            synchronize_song_timer ();
#endif // !ADT2PLAY

    if (song_timer > 3600-1)
    {
      song_timer = 0;
      timer_temp = 0;
      song_timer_tenths = 0;
    }

#if GO32
#if ADT2PLAY
    if ((ticklooper == 0) || fast_forward)
      poll_proc ();
#else // !ADT2PLAY
    if (ticklooper > 0)
    {
      if ((fast_forward || _rewind) && !replay_forbidden)
        poll_proc ();
    }
    else
      if (!replay_forbidden)
        poll_proc ();
#endif // !ADT2PLAY

    if (macro_ticklooper == 0)
      macro_poll_proc ();

    ticklooper++;
    if (ticklooper >= (IRQ_freq / tempo))
      ticklooper = 0;

    macro_ticklooper++;
    if (macro_ticklooper >= (IRQ_freq / (tempo * _macro_speedup ())))
      macro_ticklooper = 0;
#endif // GO32
#if ADT2PLAY
  }
#endif // ADT2PLAY

  DBG_LEAVE (); //EXIT //timer_poll_proc
}
