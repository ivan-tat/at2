// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Return value: LSB = end, MSB = start (all 0-31)
uint16_t VGA_GetCursorShape (void) {
  uint_least8_t x;

  outportb (VGA_CRTC_ADDR_PORT, 0x0A); // Cursor Start Register
  // Bits 7-6 - unused
  // Bit  5   - Cursor Disable: 0 = enable, 1 = disable
  // Bits 4-0 - Cursor Scan Line Start: 0-31
  x = inportb (VGA_CRTC_DATA_PORT);

  outportb (VGA_CRTC_ADDR_PORT, 0x0B); // Cursor End Register
  // Bit  7   - unused
  // Bits 6-5 - Cursor Skew: different behavior (better set to 0)
  // Bits 4-0 - Cursor Scan Line End: 0-31
  v_curshape = (inportb (VGA_CRTC_DATA_PORT) + (x << 8)) & 0x1F1F;

  return v_curshape;
}
