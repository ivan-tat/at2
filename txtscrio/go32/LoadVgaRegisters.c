// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void LoadVgaRegisters (VGA_REG_DATA *reg) {
  uint8_t i;

  // Clear write protection of CRTC registers 0-7
  // Vertical Retrace End Register:
  // Bit 7    - Protect: CRTC Registers 0-7 Protect Enable (0 = disable)
  // Bit 6    - Bandwidth: Memory Refresh Bandwidth
  // Bits 5-4 - unused
  // Bits 3-0 - Vertical Retrace End
  outportb (VGA_CRTC_ADDR_PORT, 0x11);
  outportb (VGA_CRTC_DATA_PORT, inportb (VGA_CRTC_DATA_PORT) & 0x7F);

  for (i = 0; i < 29; i++)
    out_reg (reg[i]);
}
