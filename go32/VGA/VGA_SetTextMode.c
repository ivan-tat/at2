// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void VGA_SetTextMode (uint8_t mode, uint8_t font, uint8_t page) {
  uint16_t orig_fs;

  VBIOS_set_video_mode (mode);

  switch (font) {
  case 0:
  default:
    break;
  case 8:
    VBIOS_load_ROM_font_8x8 (0, true);
    break;
  case 14:
    VBIOS_load_ROM_font_8x14 (0, true);
    break;
  case 16:
    VBIOS_load_ROM_font_8x16 (0, true);
    break;
  }

  VBIOS_set_active_video_page (page);

  orig_fs = _fargetsel ();
  _farsetsel (_dos_ds);

  v_mode = BDA_get_active_video_mode ();
  v_font = BDA_get_screen_character_height ();
  v_cols = BDA_get_screen_text_columns ();
  v_rows = BDA_get_screen_text_rows ();
  v_page = BDA_get_active_video_page ();
  v_regen_size = BDA_get_video_regen_buffer_size ();
  v_ofs = BDA_get_video_page_offset ();
  v_seg = (v_mode == 7) ? VGA_SEG_B000 : VGA_SEG_B800;

  _farsetsel (orig_fs);

  v_curpos = VBIOS_get_cursor_pos (v_page);
  VGA_GetCursorShape ();
}
