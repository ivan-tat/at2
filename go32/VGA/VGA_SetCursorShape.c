// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void VGA_SetCursorShape (uint16_t shape) {
  outportb (VGA_CRTC_ADDR_PORT, 0x0A); // Cursor Start Register
  // Bits 7-6 - unused
  // Bit  5   - Cursor Disable: 0 = enable, 1 = disable
  // Bits 4-0 - Cursor Scan Line Start: 0-31
  outportb (VGA_CRTC_DATA_PORT,
            (inportb (VGA_CRTC_DATA_PORT) & ~0x1F) | (shape >> 8));

  outportb (VGA_CRTC_ADDR_PORT, 0x0B); // Cursor End Register
  // Bit  7   - unused
  // Bits 6-5 - Cursor Skew: different behavior (better set to 0)
  // Bits 4-0 - Cursor Scan Line End: 0-31
  outportb (VGA_CRTC_DATA_PORT,
            (inportb (VGA_CRTC_DATA_PORT) & ~0x1F) | (shape & 0xFF));
}
