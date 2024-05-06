// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void VGA_SetOffset (uint16_t offset) {
  outportb (VGA_CRTC_ADDR_PORT, 0x13); // Offset Register
  // Bits 7-0 - Offset: Width / (MemoryAddressSize * 2)
  outportb (VGA_CRTC_DATA_PORT, offset >> 1);
}
