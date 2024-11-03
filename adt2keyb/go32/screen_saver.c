// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static struct {
  uint8_t r, g, b;
} temp_buf[32][255]; // HINT: (FPC) start index 1,1

static int min0 (int x) {
  return (x <= 0) ? 0 : x;
}

static void fadeout (void) {
  int d; // depth

  DBG_ENTER ("fadeout");

  for (d = 0; d < 32; d++) {
    int i;

    for (i = 0; i < 255; i++) {
      uint8_t r, g, b;

      GetRGBitem (i + 1, &r, &g, &b);
      temp_buf[d][i].r = r;
      temp_buf[d][i].g = g;
      temp_buf[d][i].b = b;
      SetRGBitem (i + 1, min0 (r - 1), min0 (g - 1), min0 (b - 1));
    }

    WaitRetrace ();

    realtime_gfx_poll_proc ();

    if (!((d + 1) % 4))
      draw_screen ();

    keyboard_reset_buffer ();
  }

  DBG_LEAVE (); //fadeout
}

static void fadein (void) {
  int d; // depth

  DBG_ENTER ("fadein");

  for (d = 31; d >= 0; d--) {
    int i;

    for (i = 0; i < 255; i++)
      SetRGBitem (i, temp_buf[d][i].r,
                     temp_buf[d][i].g,
                     temp_buf[d][i].b);

    if ((d + 1) % 4)
      WaitRetrace ();

    realtime_gfx_poll_proc ();

    if (!((d + 1) % 4))
      draw_screen ();

    keyboard_reset_buffer ();
  }

  DBG_LEAVE (); //fadein
}

void screen_saver (void) {
  DBG_ENTER ("screen_saver");

  if (ssaver_time) {
    fadeout ();
    do {
      realtime_gfx_poll_proc ();
      draw_screen ();
    } while (seconds_counter);
    fadein ();
  }

  DBG_LEAVE (); //screen_saver
}
