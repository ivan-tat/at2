// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure draw_screen;

const
   frame_start: Longint = 0;
   frame_end: Longint = 0;
   actual_frame_end: Longint = 0;

begin
  realtime_gfx_poll_proc;
  draw_screen_proc;
  If _update_sdl_screen then SDL_Flip(screen);

  If _draw_screen_without_delay then _draw_screen_without_delay := FALSE
  else begin // keep framerate
         actual_frame_end := SDL_GetTicks;
         frame_end := frame_start+(1000 DIV sdl_frame_rate);
         // always sleep at least 2 msec
         If (actual_frame_end+2 > frame_end) then frame_end := actual_frame_end+2;
         SDL_Delay(frame_end-actual_frame_end);
         frame_start := SDL_GetTicks;
       end;
end;
