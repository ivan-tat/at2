// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// row: 0-31
void VGA_SetPresetRowScan (uint8_t row) {
  outportb (VGA_CRTC_ADDR_PORT, 0x08); // Preset Row Scan Register
  // Bit  7   - unused
  // Bits 6-5 - Byte Panning
  // Bits 4-0 - Preset Row Scan: 0-31
  //            Valid values range from 0 to the value of the
  //            `Maximum Scan Line' field

  outportb (VGA_CRTC_DATA_PORT,
            (inportb (VGA_CRTC_DATA_PORT) & ~0x1F) | row);
}
