// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void draw_screen (void)
{
  int32_t frame_start = 0;
  int32_t frame_end = 0;
  int32_t actual_frame_end = 0;

  realtime_gfx_poll_proc ();
  draw_screen_proc ();
  if (_update_sdl_screen)
    SDL_Flip (screen);

  if (_draw_screen_without_delay)
    _draw_screen_without_delay = false;
  else
  {
    // keep framerate
    actual_frame_end = SDL_GetTicks ();
    frame_end = frame_start + (1000 / sdl_frame_rate);
    // always sleep at least 2 msec
    if (frame_end < actual_frame_end + 2)
      frame_end = actual_frame_end + 2;
    SDL_Delay (frame_end - actual_frame_end);
    frame_start = SDL_GetTicks ();
  }
}
