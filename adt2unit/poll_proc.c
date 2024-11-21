// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void poll_proc (void)
{
  DBG_ENTER ("poll_proc");

  if (((!pattern_delay) && ((ticks - tick0 + 1) >= speed))
#if ADT2PLAY
      || fast_forward)
#else // !ADT2PLAY
      || fast_forward || _rewind || single_play)
#endif // !ADT2PLAY
  {
#if !ADT2PLAY
    if (debugging
        && (!single_play) && (!pattern_break)
        && ((!space_pressed) || no_step_debugging))
    {
      DBG_LEAVE (); //poll_proc
      return;
    }
#endif // !ADT2PLAY

#if !ADT2PLAY
    if (   (!single_play)
        && (!play_single_patt))
    {
#endif // !ADT2PLAY
      if (songdata.pattern_order[current_order] >= 0x80)
        if (calc_order_jump () == -1)
        {
          DBG_LEAVE (); //poll_proc
          return;
        }
      current_pattern = songdata.pattern_order[current_order];
#if !ADT2PLAY
    }
#endif // !ADT2PLAY

    play_line ();

#if ADT2PLAY
    if (!fast_forward)
#else // !ADT2PLAY
    if ((!single_play) && !(fast_forward || _rewind))
#endif // !ADT2PLAY
      update_effects ();
    else
      for (uint8_t i = 1; i <= speed; i++)
      {
        update_effects ();

        if ((i % 4) == i) // FIXME: maybe `== 0' ?
          update_extra_fine_effects ();

        ticks++;
      }

#if ADT2PLAY
    pattern_break_docmd = pattern_break;
    pattern_break_oldord = current_order;
    if (fast_forward || !pattern_delay)
#else // !ADT2PLAY
    tick0 = ticks;
    if (   (!single_play)
        && (fast_forward || _rewind || !pattern_delay))
#endif // !ADT2PLAY
      update_song_position ();

#if ADT2PLAY
    if (pattern_break_docmd)
      pattern_break_loop = current_order == pattern_break_oldord;

    tick0 = ticks;
    if (fast_forward)
#else // !ADT2PLAY
    if (fast_forward || _rewind)
#endif // !ADT2PLAY
      if (!pattern_delay)
        synchronize_song_timer ();

#if ADT2PLAY
    if (fast_forward && pattern_delay)
#else // !ADT2PLAY
    if ((fast_forward || _rewind) && pattern_delay)
#endif // !ADT2PLAY
    {
      tickD = 0;
      pattern_delay = false;
    }

#if !ADT2PLAY
    if (fast_forward || _rewind)
    {
      double factor;

#if GO32
      keyboard_reset_buffer ();
#endif // GO32

      factor = 1 / (5 - (fast_forward ? fforward_factor : rewind_factor) + 1);
      if (fabs (time_playing - time_playing0) > (factor * (1 + (tempo / 255.0)) / speed))
      {
        fast_forward = false;
        _rewind = false;
        time_playing0 = time_playing;
        synchronize_song_timer ();
      }
    }
#endif // !ADT2PLAY
  }
  else
  {
    update_effects ();
    ticks++;

#if !ADT2PLAY
    if (!(   debugging
          && (!single_play)
          && ((!space_pressed) || no_step_debugging)))
    {
#endif // !ADT2PLAY
      if (pattern_delay && (tickD > 1))
        tickD--;
      else
      {
#if ADT2PLAY
        if (pattern_delay)
#else // !ADT2PLAY
        if (pattern_delay && !single_play)
#endif // !ADT2PLAY
        {
          tick0 = ticks;
          update_song_position ();
        }
        pattern_delay = false;
      }
#if !ADT2PLAY
    }
#endif // !ADT2PLAY
  }

  tickXF++;
  if (!(tickXF % 4))
  {
    update_extra_fine_effects ();
    tickXF -= 4;
  }

  DBG_LEAVE (); //EXIT //poll_proc
}
