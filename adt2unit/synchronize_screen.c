// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*static*/ void synchronize_screen (void)
{
  DBG_ENTER ("synchronize_screen");

#if GO32
  if (mouse_active)
    Mouse_GetPos (&scr_scroll_x, &scr_scroll_y);

  if (   (scr_scroll_x != old_scr_scroll_x)
      || (scr_scroll_y != old_scr_scroll_y))
  {
    bool update = false;
    uint16_t x_max = scr_font_width * MaxCol - scr_font_width * hard_maxcol;
    uint16_t y_max = scr_font_height * MaxLn - scr_font_height * hard_maxln;

    old_scr_scroll_x = scr_scroll_x;
    old_scr_scroll_y = scr_scroll_y;

    if (scr_scroll_x > x_max)
    {
      scr_scroll_x = x_max;
      update = true;
    }

    if (scr_scroll_y > y_max)
    {
      scr_scroll_y = y_max;
      update = true;
    }

    if (update)
      update_mouse_position ();

    WaitRetrace ();
    if (!is_VESA_emulated_mode ())
      SetTextDisp (scr_scroll_x, scr_scroll_y);
    else
      virtual_screen__first_row = scr_scroll_y * 800;
  }
#else // !GO32
  if (is_default_screen_mode ())
  {
    uint16_t y_max = (16 * MaxLn - 16 * hard_maxln);

    if (screen_scroll_offset > y_max)
      screen_scroll_offset = y_max;

    virtual_screen__first_row = screen_scroll_offset * SCREEN_RES_X;
  }
#endif // !GO32

  DBG_LEAVE (); //EXIT //synchronize_screen
}
