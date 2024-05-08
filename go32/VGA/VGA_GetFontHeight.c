// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

uint8_t VGA_GetFontHeight (void) {
  outportb (VGA_CRTC_ADDR_PORT, 0x09); // Maximum Scan Line Register
  // Bit 7 (SD)   - Scan Doubling
  // Bit 6 (LC9)  - Line Compare (bit 9)
  // Bit 5 (SVB9) - Start Vertical Blanking (bit 9)
  // Bits 4-0     - Maximum Scan Line (font height - 1)
  v_font = (inportb (VGA_CRTC_DATA_PORT) & 0x1F) + 1;

  return v_font;
}
