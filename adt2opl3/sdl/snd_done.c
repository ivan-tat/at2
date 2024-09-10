// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void snd_done (void) {
  unsigned i;

  SDL_PauseAudio (1);
  SDL_CloseAudio ();

  free (opl3_sample_buffer_ptr);
  opl3_sample_buffer_ptr = NULL;

  for (i = 0; i < 18; i++)
    if (opl3_sample_buffer_chan_ptr[i]) {
      free (opl3_sample_buffer_chan_ptr[i]);
      opl3_sample_buffer_chan_ptr[i] = NULL;
    }
}
