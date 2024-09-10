// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void snd_init (void) {
  unsigned i;

  opl3_sample_buffer_ptr = malloc (sdl_sample_buffer*4);
  // FIXME: check `opl3_sample_buffer_ptr'

  for (i = 0; i < 18; i++)
    opl3_sample_buffer_chan_ptr[i] = malloc (sdl_sample_buffer*4);
    // FIXME: check `opl3_sample_buffer_chan_ptr[i]'

  sample_frame_size = (int64_t) (sdl_sample_rate / 50.0
                                 * (1 + sdl_timer_slowdown / 100.0));

  opl3_init ();

  sdl_audio_spec.freq = sdl_sample_rate;
  sdl_audio_spec.format = AUDIO_S16;
  sdl_audio_spec.channels = 2;
  sdl_audio_spec.samples = sdl_sample_buffer;
  sdl_audio_spec.callback = &playcallback;
  sdl_audio_spec.userdata = NULL;

  if (SDL_OpenAudio (&sdl_audio_spec, NULL) < 0) {
    printf ("SDL: Audio initialization error" NL);
    exit (1);
  }

  printf ("  Sample buffer size: %hu samples (requested %d)" NL,
    (uint16_t) sdl_audio_spec.samples,
    (int32_t) sdl_sample_buffer);
  printf ("  Sampling rate: %d Hz (requested %d)" NL,
    (int) sdl_audio_spec.freq,
    (int32_t) sdl_sample_rate);

  sdl_sample_rate = sdl_audio_spec.freq;
  sdl_sample_buffer = sdl_audio_spec.samples;

  SDL_PauseAudio (0);
}
