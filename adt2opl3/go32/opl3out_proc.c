// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void opl3out_proc (uint16_t reg, uint16_t data) {
  if (_opl_regs_cache[reg] != data) {
    uint16_t port = opl3port;
    unsigned n;

    _opl_regs_cache[reg] = data;

    if (reg & 0xFF00)
      port += 2;

    outportb (port, reg);
    outportb (port + 1, data);

    for (n = 26; n; n--)
      inportb (port);
  }
}
