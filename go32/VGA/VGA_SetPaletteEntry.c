// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void VGA_SetPaletteEntry (uint8_t red, uint8_t green, uint8_t blue,
                          uint8_t index) {
  outportb (VGA_DAC_ADDR_WRITE_PORT, index);
  outportb (VGA_DAC_DATA_PORT, red);
  outportb (VGA_DAC_DATA_PORT, green);
  outportb (VGA_DAC_DATA_PORT, blue);
}
