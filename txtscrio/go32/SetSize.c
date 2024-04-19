// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void SetSize (uint16_t columns, uint16_t lines) {
#if !USE_FPC
  uint16_t orig_fs;
#endif // !USE_FPC

  outportb (VGA_CRTC_ADDR_PORT, 0x13); // Offset Register
  // Bits 7-0 - Offset: Width / (MemoryAddressSize * 2)
  outportb (VGA_CRTC_DATA_PORT, columns >> 1);

#if !USE_FPC
  orig_fs = _fargetsel ();
  _farsetsel (_dos_ds);
#endif // !USE_FPC

  BDA_set_screen_text_columns (columns);
  BDA_set_screen_text_rows (lines);
  BDA_set_video_regen_buffer_size (columns * lines * 2);

#if !USE_FPC
  _farsetsel (orig_fs);
#endif // !USE_FPC
}
