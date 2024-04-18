// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void VGA_SetStartAddress (uint16_t address) {
  outportb (VGA_CRTC_ADDR_PORT, 0x0C); // Start Address High Register
  // Bits 7-0 - Start Address High: bits 15-8 of the `Start Address' field

  outportb (VGA_CRTC_DATA_PORT, address >> 8);

  outportb (VGA_CRTC_ADDR_PORT, 0x0D); // Start Address Low Register
  // Bits 7-0 - Start Address Low: bits 7-0 of the `Start Address' field

  outportb (VGA_CRTC_DATA_PORT, address & 0xFF);
}
