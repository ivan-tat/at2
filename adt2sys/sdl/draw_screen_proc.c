// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void draw_screen_proc (void)
{
  _update_sdl_screen = false;
  if (Compare (screen_ptr, ptr_screen_mirror, (SCREEN_RES_X / scr_font_width) * MAX_ROWS * 2))
    return; //draw_screen_proc
  else
    ScreenMemCopy (screen_ptr, ptr_screen_mirror);

  _cursor_blink_factor = ((int)sdl_frame_rate * 13 + 50) / 100; // +50 for rounding
  _update_sdl_screen = true;

#if i386
  switch (program_screen_mode)
  {
    case 0: draw_SDL_screen_720x480 (); break;
    case 1: draw_SDL_screen_960x800 (); break;
    case 2: draw_SDL_screen_1440x960 (); break;
    default: break;
  }
#else // !i386
  switch (program_screen_mode)
  {
    case 0: draw_SDL_screen (90, 40, 720 * 480); break;
    case 1: draw_SDL_screen (120, 50, 960 * 800); break;
    case 2: draw_SDL_screen (180, 60, 1440 * 960); break;
    default: break;
  }
#endif // !i386

  //EXIT //draw_screen_proc
}
