// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void vid_SetVideoMode (bool do_delay)
{
  SDL_RWops *rwop;
  SDL_Surface *icon;

  if (do_delay) SDL_Delay (1000);
  screen = SDL_SetVideoMode (SCREEN_RES_X, SCREEN_RES_Y, 8, SDL_SWSURFACE);
  if (screen == NULL)
  {
    printf ("SDL: Couldn't initialize video mode" NL);
    exit (1);
  }

  vid_SetRGBPalette (rgb_color);
  memmove (rgb_color_alt, rgb_color, sizeof (rgb_color));
  _FrameBuffer = screen->pixels;
  rwop = SDL_RWFromMem (adt2_icon_bitmap, sizeof (adt2_icon_bitmap));
  icon = SDL_LoadBMP_RW (rwop, true);
  SDL_WM_SetIcon (icon, NULL);
  SDL_WM_SetCaption ("/|DLiB TR/|CK3R ][ SDL", NULL);
}
