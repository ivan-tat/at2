// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void VGA_SetPixelPanningMode (bool mode) {
  uint_least8_t x;

  inportb (VGA_STATUS_1_PORT);

  outportb (VGA_ATTR_WRITE_PORT, 0x20 | 0x10); // Attribute Address Register
  // Bits 7-6 - unused
  // Bit  5   - Palette Address Source:
  //            0 = load color values to the registers in the internal palette
  //            1 = normal operation of the attribute controller
  // Bits 4-0 - Attribute Address

  // Attribute Mode Control Register (Index 10h)
  // Bit 7 (P54S)  - Palette Bits 5-4 Select
  // Bit 6 (8BIT)  - 8-bit Color Enable
  // Bit 5 (PPM)   - Pixel Panning Mode:
  //                 0 = normal operation
  //                 1 = bottom part is displayed as if `Pixel Shift Count'
  //                     and `Byte Panning' fields are set to 0
  // Bit 4         - unused
  // Bit 3 (BLINK) - Blink Enable
  // Bit 2 (LGE)   - Line Graphics Enable
  // Bit 1 (MONO)  - Monochrome Emulation
  // Bit 0 (ATGE)  - Attribute Controller Graphics Enable

  x = inportb (VGA_ATTR_READ_PORT);

  if (mode)
    x |= 0x20; // set PPM
  else
    x &= ~0x20; // clear PPM

  outportb (VGA_ATTR_WRITE_PORT, x);
}
