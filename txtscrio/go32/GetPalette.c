// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void GetPalette (void *pal, uint8_t first, uint8_t last) {
  outportb (VGA_DAC_ADDR_READ_PORT, first);
  inportsb (VGA_DAC_DATA_PORT, ((uint8_t *) pal) + first * 3,
            (last - first + 1) * 3);
}