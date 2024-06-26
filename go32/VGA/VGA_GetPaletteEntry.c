// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void VGA_GetPaletteEntry (uint8_t index, uint8_t *red, uint8_t *green,
                          uint8_t *blue) {
  outportb (VGA_DAC_ADDR_READ_PORT, index);
  *red   = inportb (VGA_DAC_DATA_PORT);
  *green = inportb (VGA_DAC_DATA_PORT);
  *blue  = inportb (VGA_DAC_DATA_PORT);
}
