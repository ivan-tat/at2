// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void draw_screen (void)
{
  if (_draw_screen_without_delay)
    _draw_screen_without_delay = false;
  else
  {
    if (do_synchronize && (_screen_refresh_pending_frames <= fps_down_factor))
      return; //draw_screen
    else
      _screen_refresh_pending_frames = 0;
  }

  if (Compare (screen_ptr, ptr_screen_mirror, (SCREEN_RES_X / scr_font_width) * MAX_ROWS * 2))
    return; //draw_screen
  else
  {
    ScreenMemCopy (screen_ptr, ptr_screen_mirror);
    if (!is_VESA_emulated_mode ())
    {
      if (!_draw_screen_without_vsync)
        WaitRetrace ();
      if ((program_screen_mode != 4) && (program_screen_mode != 5))
        dosmemput (screen_ptr, MAX_COLUMNS * MAX_ROWS * 2, v_seg * 16 + v_ofs);
      else
      {
        shift_text_screen ();
        dosmemput (ptr_temp_screen2, (SCREEN_RES_X / scr_font_width) * MAX_ROWS * 2, v_seg * 16 + v_ofs);
      }
    }
  }

  _draw_screen_without_vsync = false;

  if (is_VESA_emulated_mode ())
    switch (get_VESA_emulated_mode_idx ())
    {
      case 0:
        draw_VESA_screen_800x600_1 ();
        dump_VESA_buffer (800 * 600);
        break;

      case 1:
        draw_VESA_screen_800x600_2 ();
        dump_VESA_buffer (800 * 600);
        break;

      case 2:
        draw_VESA_screen_1024x768 ();
        dump_VESA_buffer (1024 * 768);
        break;

      default:
        break;
    }

  //EXIT //draw_screen
}
