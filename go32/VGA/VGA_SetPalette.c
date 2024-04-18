// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void VGA_SetPalette (const void *palette, uint16_t count, uint8_t index) {
  VGA_wait_while_vertical_retrace (false);

  outportb (VGA_DAC_ADDR_WRITE_PORT, index);
  outportsb (VGA_DAC_DATA_PORT, palette, count * 3);
}
