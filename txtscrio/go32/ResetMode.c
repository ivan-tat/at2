// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void ResetMode (void) {
#if !USE_FPC
  uint16_t orig_fs;
#endif // !USE_FPC

  VBIOS_set_video_mode (v_mode, v_page);

  switch (v_font) {
  case 8:
    VBIOS_load_ROM_font_8x8 (0, true);
    break;
  case 14:
    VBIOS_load_ROM_font_8x14 (0, true);
    break;
  case 16:
  default:
    VBIOS_load_ROM_font_8x16 (0, true);
    break;
  }

#if !USE_FPC
  orig_fs = _fargetsel ();
  _farsetsel (_dos_ds);
#endif // !USE_FPC

  v_font = BDA_get_screen_character_height ();
  v_cols = BDA_get_screen_text_columns ();
  v_rows = BDA_get_screen_text_rows ();
  v_regen_size = BDA_get_video_regen_buffer_size ();
  v_ofs = BDA_get_video_page_offset ();
  v_seg = VGA_SEG_B800;

#if !USE_FPC
  _farsetsel (orig_fs);
#endif // !USE_FPC

  v_curpos = VBIOS_get_cursor_pos (v_page);
  v_curshape = VGA_GetCursorShape ();

  memsetw (screen_ptr, 0x0700, MAX_SCREEN_MEM_SIZE / 2);
  dosmemput (screen_ptr,
             v_regen_size <= MAX_SCREEN_MEM_SIZE ? v_regen_size : MAX_SCREEN_MEM_SIZE,
             v_seg * 16 + v_ofs);

  MaxCol = v_cols;
  MaxLn = v_rows;
}
