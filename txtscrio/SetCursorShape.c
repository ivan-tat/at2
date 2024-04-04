// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void SetCursorShape (uint16_t shape) {
#if GO32
#if !ADT2PLAY
  virtual_cur_shape = shape;
#endif // !ADT2PLAY

  outportb (VGA_CRTC_ADDR_PORT, 0x0A);
  outportb (VGA_CRTC_DATA_PORT,
            (inportb (VGA_CRTC_DATA_PORT) & 0xE0) | (shape >> 8));

  outportb (VGA_CRTC_ADDR_PORT, 0x0B);
  outportb (VGA_CRTC_DATA_PORT,
            (inportb (VGA_CRTC_DATA_PORT) & 0xE0) | (shape & 0xFF));
#else // !GO32
  virtual_cur_shape = shape;
#endif // !GO32
}
