// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void GetPalette (void *pal, uint8_t first, uint8_t last) {
  VGA_GetPalette (first, last - first + 1, ((uint8_t *) pal) + first * 3);
}
