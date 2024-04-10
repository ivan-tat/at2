// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void VESA_GetPalette (void *Palette) {
  outportb (VGA_DAC_ADDR_READ_PORT, 0);
  inportsb (VGA_DAC_DATA_PORT, (uint8_t *) Palette, 256 * 3);
}
