// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void SetSize (uint16_t columns, uint16_t lines) {
#if !USE_FPC
  uint16_t orig_fs;
#endif // !USE_FPC

  outportb (VGA_CRTC_ADDR_PORT, 0x13);
  outportb (VGA_CRTC_DATA_PORT, columns >> 1);

#if !USE_FPC
  orig_fs = _fargetsel ();
  _farsetsel (_dos_ds);
#endif // !USE_FPC

  _farnspokew (0x44A, columns); // screen width in text columns
  _farnspokew (0x484, lines - 1); // EGA text rows - 1
  _farnspokew (0x44C, columns * lines); // length in bytes of video area

#if !USE_FPC
  _farsetsel (orig_fs);
#endif // !USE_FPC
}
