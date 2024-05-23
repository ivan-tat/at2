// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void keyboard_reset_buffer (void) {
  SDL_Event event;

  // Flush all unused events
  while (SDL_PollEvent (&event));
}
