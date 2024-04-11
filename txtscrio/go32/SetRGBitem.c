// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void SetRGBitem (uint8_t color, uint8_t red, uint8_t green, uint8_t blue) {
  VGA_SetPaletteEntry (red, green, blue, color);
}
