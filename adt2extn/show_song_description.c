// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*static*/ void show_song_description (const void *song_desc, uint8_t max_length, uint8_t max_lines)
{
  const uint8_t *desc = song_desc;
  size_t lines_count = *((size_t *)desc);
  fast_str_t *lines = (fast_str_t *)(&desc[sizeof (size_t)]);
  size_t max_len = 0; // actual maximum line width
  ssize_t first = -1, last = -1;  // first and last non-empty lines

  for (size_t i = 0; i < lines_count; i++)
  {
    if (lines[i].len != 0)
    {
      if (max_len < lines[i].len) max_len = lines[i].len;
      if (first < 0) first = i;
      last = i;
    }
  }

  if (first >= 0)
  {
    size_t count = last - first + 1;
    uint8_t xstart, ystart, len;
    ExtKeyCode fkey;
    String_t s, t;

    if (max_len > max_length) max_len = max_length;
    if (max_len < 20) max_len = 20;
    if (count > max_lines) count = max_lines;

    ScreenMemCopy (screen_ptr, ptr_screen_backup);
    centered_frame_vdest = screen_ptr;
    t = iCASE (StrToString ((String *)&s, " Song description ", sizeof (s) - 1));
    centered_frame (&xstart, &ystart,
                    max_len + 1, count + 2,
                    (String *)&t,
                    dialog_background + dialog_border,
                    dialog_background + dialog_title,
                    frame_double);
    t = iCASE (StrToString ((String *)&s, " ~C~ontinue ", sizeof (s) - 1));
    len = CStrLen ((String *)&s);
    ShowCStr (screen_ptr,
              xstart + ((max_len - len) / 2) + 1, ystart + count + 1,
              (String *)&t,
              dialog_sel_itm_bck + dialog_sel_itm,
              dialog_sel_itm_bck + dialog_sel_short);

    for (size_t i = 0; i < count; i++)
    {
      StrToString ((String *)&s, (char *)(lines[first + i].data), sizeof (s) - 1);
      t = FilterStr2 ((String *)&s, _valid_characters, '_');
      ShowStr (screen_ptr,
               xstart + 1, ystart + 1 + i,
               (String *)&t,
               dialog_background + dialog_context_dis);
    }

    // wait for a specific key press
    do fkey = GetKey (); while ((fkey != kESC) && (fkey != kENTER) && (fkey != kAltC));

    move_to_screen_data = ptr_screen_backup;
    move_to_screen_area[0] = xstart;
    move_to_screen_area[1] = ystart;
    move_to_screen_area[2] = xstart + max_len + 1 + 2;
    move_to_screen_area[3] = ystart + count + 2 + 1;
    move2screen ();
    no_status_refresh = true;
  }
}
