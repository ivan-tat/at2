// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#define fade_first 0
#define fade_last 255

void VgaFade (tFADE_BUF *data, tFADE fade, tDELAY dly) {
  int i, j;

  if ((fade == fadeOut)
  &&  ((data->action == fadeFirst) || (data->action == fadeIn))) {
    GetPalette (data->pal0, fade_first, fade_last);
    if (dly == delayed) {
      for (i = fade_speed; i >= 0; i--) {
        for (j = fade_first; j <= fade_last; j++) {
          data->pal1[j].r = data->pal0[j].r * i / fade_speed;
          data->pal1[j].g = data->pal0[j].g * i / fade_speed;
          data->pal1[j].b = data->pal0[j].b * i / fade_speed;
        }
        SetPalette (data->pal1, fade_first, fade_last);
#if ADT2PLAY
        WaitRetrace ();
#else // !ADT2PLAY
        delay (1);
#endif // !ADT2PLAY
      }
    } else {
      memset (data->pal1, 0, sizeof(data->pal1));
      SetPalette (data->pal1, fade_first, fade_last);
    }
    data->action = fadeOut;
  }

  if ((fade == fadeIn) && (data->action == fadeOut)) {
    if (dly == delayed) {
      for (i = 0; i <= fade_speed; i++) {
        for (j = fade_first; j <= fade_last; j++) {
          data->pal1[j].r = data->pal0[j].r * i / fade_speed;
          data->pal1[j].g = data->pal0[j].g * i / fade_speed;
          data->pal1[j].b = data->pal0[j].b * i / fade_speed;
        }
        SetPalette (data->pal1, fade_first, fade_last);
#if ADT2PLAY
        WaitRetrace ();
#else // !ADT2PLAY
        delay (1);
#endif // !ADT2PLAY
      }
    } else
      SetPalette (data->pal0, fade_first, fade_last);

    data->action = fadeIn;
  }
}
