// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void vid_FadeOut (void)
{
  for (uint8_t i = 1; i < 16; i++)
  {
      rgb_color_alt[i].r = rgb_color_alt[i].r - 1 < 0 ? 0 : rgb_color_alt[i].r - 1;
      rgb_color_alt[i].g = rgb_color_alt[i].g - 1 < 0 ? 0 : rgb_color_alt[i].g - 1;
      rgb_color_alt[i].b = rgb_color_alt[i].b - 1 < 0 ? 0 : rgb_color_alt[i].b - 1;
  }
  SDL_SetPalette (screen, SDL_PHYSPAL, (SDL_Color *) &rgb_color_alt, 0, 16);
}
