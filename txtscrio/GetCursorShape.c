// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

uint16_t GetCursorShape (void) {
#if GO32
  uint_least8_t x;

  outportb (VGA_CRTC_ADDR_PORT, 0x0A);
  x = inportb (VGA_CRTC_DATA_PORT);

  outportb (VGA_CRTC_ADDR_PORT, 0x0B);
  return (inportb (VGA_CRTC_DATA_PORT) + (x << 8)) & 0x1F1F;
#else // !GO32
  return virtual_cur_shape;
#endif // !GO32
}
