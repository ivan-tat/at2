// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void opl3exp (uint16_t data) {
  if (_opl_regs_cache[(data & 0xFF) | 0x100] != (data >> 8)) {
    uint16_t port = opl3port + 2;
    unsigned n;

    _opl_regs_cache[(data & 0xFF) | 0x100] = data >> 8;

    outportb (port, data);

    for (n = 6; n; n--)
      inportb (port);

    port++;

    outportb (port, data >> 8);

    for (n = 36; n; n--)
      inportb (port);
  }
}
