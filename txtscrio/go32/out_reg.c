// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void out_reg (VGA_REGISTER *reg) {
  switch (reg->port) {
  case VGA_ATTR_WRITE_PORT:
    inportb (VGA_STATUS_1_PORT);
    outportb (VGA_ATTR_WRITE_PORT, reg->idx | 0x20);
    outportb (VGA_ATTR_WRITE_PORT, reg->val);
    break;
  case VGA_MISC_WRITE_PORT:
  case VGA_ENABLE_PORT:
    outportb (reg->port, reg->val);
    break;
  case VGA_SEQ_ADDR_PORT:
  case VGA_GRAPH_ADDR_PORT:
  case VGA_CRTC_ADDR_PORT:
  default:
    outportb (reg->port, reg->idx);
    outportb (reg->port + 1, reg->val);
    break;
  }
}
