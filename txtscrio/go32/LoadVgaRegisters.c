// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void LoadVgaRegisters (VGA_REG_DATA *reg) {
  uint8_t t, i;

  outportb (VGA_CRTC_ADDR_PORT, 0x11);
  t = inportb (VGA_CRTC_DATA_PORT) & 0x7F;

  outportb (VGA_CRTC_ADDR_PORT, 0x11);
  outportb (VGA_CRTC_DATA_PORT, t);

  for (i = 0; i < 29; i++)
    out_reg (reg[i]);
}
