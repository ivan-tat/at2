// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void fade_out_playback (bool fade_screen)
{
  DBG_ENTER ("fade_out_playback");

#if GO32

  tFADE_BUF fade_buf;

  if (fade_screen)
  {
    draw_screen ();
    fade_buf.action = fadeFirst;
    fade_speed = 32;
    GetPalette (fade_buf.pal0, 0, 255);

    for (int_least16_t i = fade_speed; i >= 0; i--)
    {
      for (int_least16_t j = 0; j <= 255; j++)
      {
        fade_buf.pal1[j].r = fade_buf.pal0[j].r * i / fade_speed;
        fade_buf.pal1[j].g = fade_buf.pal0[j].g * i / fade_speed;
        fade_buf.pal1[j].b = fade_buf.pal0[j].b * i / fade_speed;
      }

      SetPalette (fade_buf.pal1, 0, 255);
      if (play_status != isStopped)
      {
        fade_out_volume = i * 2;
        set_global_volume ();
        delay (fade_out_volume / 32);
      }
      else
        delay (1);

      realtime_gfx_poll_proc ();
      draw_screen ();
      keyboard_reset_buffer ();
    }

    fade_buf.action = fadeIn;
  }
  else if (play_status != isStopped)
    for (int_least8_t i = 63; i >= 0; i--)
    {
      fade_out_volume = i;
      set_global_volume ();
      delay (fade_out_volume / 32);
      realtime_gfx_poll_proc ();
      keyboard_reset_buffer ();
    }

#else // !GO32

  uint_least16_t factor = fade_screen ? 255 : 63;
  uint_least16_t n = (global_volume > 0) ? factor / global_volume : 0;
  uint_least16_t i = 0;

  fade_out_volume = 63;

  if (play_status != isStopped)
    for (uint_least16_t j = 1; j <= factor; j++)
    {
      i++;
      if (i > n)
      {
        i = 0;
        fade_out_volume--;
        set_global_volume ();
        if (fade_screen || (!(j % 5)))
        {
          _draw_screen_without_delay = true;
          draw_screen ();
          keyboard_reset_buffer ();
        }
      }
      if (fade_screen)
      {
        vid_FadeOut ();
        SDL_Delay (1);
      }
    }
  else
  {
    for (uint_least16_t i = 1; i <= 255; i++)
    {
      fade_out_volume = i;
      if (fade_screen)
      {
        vid_FadeOut ();
        SDL_Delay (1);
      }
    }
  }

#endif // !GO32

  DBG_LEAVE (); //EXIT //fade_out_playback
}
