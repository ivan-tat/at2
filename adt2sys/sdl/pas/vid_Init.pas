// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure vid_Init;
begin
  SDL_Init(SDL_INIT_VIDEO or SDL_INIT_TIMER or SDL_INIT_AUDIO);
end;
