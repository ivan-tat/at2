// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void VGA_SetLineCompare (uint16_t line) {
  uint_least8_t x;

  outportb (VGA_CRTC_ADDR_PORT, 0x18); // Line Compare Register
  // Bits 7-0 - bits 7-0 of the `Line Compare' field

  outportb (VGA_CRTC_DATA_PORT, line & 0xFF);

  outportb (VGA_CRTC_ADDR_PORT, 0x07); // Overflow Register
  // Bit 7 (VRS9) - Vertical Retrace Start (bit 9)
  // Bit 6 (VDE9) - Vertical Display End (bit 9)
  // Bit 5 (VT9)  - Vertical Total (bit 9)
  // Bit 4 (LC8)  - Line Compare (bit 8)
  // Bit 3 (SVB8) - Start Vertical Blanking (bit 8)
  // Bit 2 (VRS8) - Vertical Retrace Start (bit 8)
  // Bit 1 (VDE8) - Vertical Display End (bit 8)
  // Bit 0 (VT8)  - Vertical Total (bit 8)

  x = inportb (VGA_CRTC_DATA_PORT);

  if (line & 0x100)
    x |= 0x10; // set LC8
  else
    x &= ~0x10; // clear LC8

  outportb (VGA_CRTC_DATA_PORT, x);

  outportb (VGA_CRTC_ADDR_PORT, 0x09); // Maximum Scan Line Register
  // Bit 7 (SD)   - Scan Doubling
  // Bit 6 (LC9)  - Line Compare (bit 9)
  // Bit 5 (SVB9) - Start Vertical Blanking (bit 9)
  // Bits 4-0     - Maximum Scan Line

  x = inportb (VGA_CRTC_DATA_PORT);

  if (line & 0x200)
    x |= 0x40; // set LC9
  else
    x &= ~0x40; // clear LC9

  outportb (VGA_CRTC_DATA_PORT, x);
}
