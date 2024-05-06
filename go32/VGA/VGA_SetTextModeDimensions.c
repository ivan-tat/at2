// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void VGA_SetTextModeDimensions (uint8_t cols, uint8_t rows, uint16_t size) {
  uint16_t orig_fs;

  v_cols = cols;
  v_rows = rows;
  v_regen_size = size;

  orig_fs = _fargetsel ();
  _farsetsel (_dos_ds);

  BDA_set_screen_text_columns (cols);
  BDA_set_screen_text_rows (rows);
  BDA_set_video_regen_buffer_size (size);

  _farsetsel (orig_fs);
}
