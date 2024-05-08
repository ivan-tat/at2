// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Return value: LSB = column, MSB = row (all 0-255)
uint16_t VGA_GetCursorPosition (void) {
  uint_least16_t loc;

  outportb (VGA_CRTC_ADDR_PORT, 0x0E); // Cursor Location High Register
  // Bits 7-0 - bits 15-8 of the `Cursor Location' field
  loc = inportb (VGA_CRTC_DATA_PORT) << 8;

  outportb (VGA_CRTC_ADDR_PORT, 0x0F); // Cursor Location Low Register
  // Bits 7-0 - bits 7-0 of the `Cursor Location' field
  loc += inportb (VGA_CRTC_DATA_PORT);

  v_curpos = (loc % v_cols) + ((loc / v_cols) << 8);

  return v_curpos;
}
