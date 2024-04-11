// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void VGA_GetPalette (uint8_t index, uint16_t count, void *palette) {
  outportb (VGA_DAC_ADDR_READ_PORT, index);
  inportsb (VGA_DAC_DATA_PORT, palette, count * 3);
}
