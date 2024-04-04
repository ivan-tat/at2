// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void Split2Static (void) {
  inportb (VGA_STATUS_1_PORT);
  outportb (VGA_ATTR_WRITE_PORT, 0x10 | 0x20);
  outportb (VGA_ATTR_WRITE_PORT, inportb (VGA_ATTR_READ_PORT) | 0x20);
}
