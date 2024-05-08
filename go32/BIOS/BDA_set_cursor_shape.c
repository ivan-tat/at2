// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// top, bottom: 0-31
static __inline__ void BDA_set_cursor_shape (uint8_t top, uint8_t bottom) {
  _farnspokew (0x460, bottom + (top << 8));
  // LSB: Cursor ending (bottom) scan line
  // MSB: Cursor starting (top) scan line
}
