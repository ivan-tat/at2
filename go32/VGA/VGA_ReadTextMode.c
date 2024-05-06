// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void VGA_ReadTextMode (void) {
  uint16_t orig_fs;

  VBIOS_get_video_mode (&v_mode, &v_page);

  orig_fs = _fargetsel ();
  _farsetsel (_dos_ds);

  v_font = BDA_get_screen_character_height ();
  v_cols = BDA_get_screen_text_columns ();
  v_rows = BDA_get_screen_text_rows ();
  v_regen_size = BDA_get_video_regen_buffer_size ();
  v_ofs = BDA_get_video_page_offset ();
  v_seg = (v_mode == 7) ? VGA_SEG_B000 : VGA_SEG_B800;

  _farsetsel (orig_fs);

  v_curpos = VBIOS_get_cursor_pos (v_page);
  v_curshape = VGA_GetCursorShape ();
}
