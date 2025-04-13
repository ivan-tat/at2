// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void vid_SetRGBPalette (void *palette)
{
  SDL_SetPalette (screen, SDL_PHYSPAL, (SDL_Color *)palette, 0, 16);
}
