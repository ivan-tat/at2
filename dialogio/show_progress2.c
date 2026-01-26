// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// If `refresh_dif' < 0: acts like `show_progress (value)'
void show_progress2 (int32_t value, int32_t refresh_dif)
{
  char s[40+2+1]; // + at most 2 color switches
  String t[40+2+1]; // + at most 2 color switches

  DBG_ENTER ("show_progress2");

  if ((progress_num_steps != 0) && (progress_value != 0))
  {
    if ((uint32_t) value != UINT32_NULL)
    {
      bool flag;

      if (progress_num_steps == 1)
        progress_new_value = (int32_t) (40.0 * value / progress_value);
      else
        progress_new_value = (int32_t)
          (  40.0 * (progress_step - 1) / progress_num_steps
           + 40.0 * value / progress_value / progress_num_steps);
      progress_new_value = max (progress_new_value, 40);

      if (refresh_dif < 0)
        flag = progress_new_value != progress_old_value;
      else
        flag =  (abs (progress_new_value - progress_old_value) >= refresh_dif)
             || (progress_new_value == 40);

      if (flag)
      {
        progress_old_value = progress_new_value;

        snprintf (s, sizeof (s), "%4" PRIu8 "%%", (uint8_t) (progress_new_value * 100.0 / 40.0));
        StrToString (t, s, sizeof (t) - 1);
        ShowStr (screen_ptr, progress_xstart + 35, progress_ystart - 1, t,
                 dialog_background + dialog_hi_text);

        s[0] = '~';
        if (progress_new_value > 0)
          memset (s + 1, charmap.full_block, progress_new_value); // '█'
        s[1 + progress_new_value] = '~';
        if (progress_new_value < 40)
          memset (s + 1 + progress_new_value + 1, charmap.medium_shade, 40 - progress_new_value); // '░'
        s[40+2] = '\0'; // terminate string
        StrToString (t, s, sizeof (t) - 1);
        ShowCStr (screen_ptr, progress_xstart, progress_ystart, t,
                  dialog_background + dialog_prog_bar1,
                  dialog_background + dialog_prog_bar2);

        realtime_gfx_poll_proc ();
        draw_screen ();
      }
    }
    else
    {
      StrToString (t, "   0%", sizeof (t) - 1);
      ShowStr (screen_ptr, progress_xstart + 35, progress_ystart - 1, t,
               dialog_background + dialog_hi_text);

      memset (s, charmap.medium_shade, 40); // '░'
      s[40] = '\0'; // terminate string
      StrToString (t, s, sizeof (t) - 1);
      ShowStr (screen_ptr, progress_xstart, progress_ystart, t,
               dialog_background + dialog_prog_bar1);

      realtime_gfx_poll_proc ();
      draw_screen ();
    }
  }

  DBG_LEAVE (); //EXIT //show_progress2
}
