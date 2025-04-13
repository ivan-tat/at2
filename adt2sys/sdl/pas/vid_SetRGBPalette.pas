// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure vid_SetRGBPalette(var palette);
begin
  SDL_SetPalette(screen,SDL_PHYSPAL,SDL_ColorArray(palette),0,16);
end;
