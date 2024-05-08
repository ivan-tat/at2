// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void OnMakeTextMode (bool load_font) {
  uint16_t orig_fs;
  int i;

  /*** Update BIOS Data Area ***/

  orig_fs = _fargetsel ();
  _farsetsel (_dos_ds);

  BDA_set_active_video_mode (v_mode);
  BDA_set_screen_character_height (v_font);
  BDA_set_screen_text_columns (v_cols);
  BDA_set_screen_text_rows (v_rows);
  BDA_set_active_video_page (v_page);
  BDA_set_video_regen_buffer_size (v_regen_size);
  BDA_set_video_page_offset (v_ofs);
  for (i = 0; i < 8; i++)
    if (i == v_page)
      BDA_set_cursor_position (i, v_curpos & 0xFF, v_curpos >> 8);
    else
      BDA_set_cursor_position (i, 0, 0);
  BDA_set_cursor_shape (v_curshape >> 8, v_curshape & 0xFF);

  _farsetsel (orig_fs);

  if (load_font)
    switch (v_font) {
    case 8:
      VBIOS_load_ROM_font_8x8 (0, 0);
      break;
    case 14:
      VBIOS_load_ROM_font_8x14 (0, 0);
      break;
    case 16:
      VBIOS_load_ROM_font_8x16 (0, 0);
      break;
    default:
      break;
    }
}
