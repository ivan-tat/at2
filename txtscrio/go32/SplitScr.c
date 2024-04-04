// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void SplitScr (uint16_t line) {
  uint8_t x;

  outportb (VGA_CRTC_ADDR_PORT, 0x18);
  outportb (VGA_CRTC_DATA_PORT, line & 0xFF);

  outportb (VGA_CRTC_ADDR_PORT, 0x07);
  x = inportb (VGA_CRTC_DATA_PORT);
  if (line < 0x100)
    x &= 0xEF;
  else
    x |= 0x10;
  outportb (VGA_CRTC_DATA_PORT, x);

  outportb (VGA_CRTC_ADDR_PORT, 0x09);
  x = inportb (VGA_CRTC_DATA_PORT);
  if (line < 0x200)
    x &= 0xBF;
  else
    x |= 0x40;
  outportb (VGA_CRTC_DATA_PORT, x);
}
