// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void opl2out (uint16_t reg, uint16_t data) {
  if (_opl_regs_cache[reg] != data) {
    uint16_t port = opl3port;
    unsigned n;

    _opl_regs_cache[reg] = data;

    if (reg & 0xFF00)
      port += 2;

    outportb (port, reg);

    for (n = 6; n; n--)
      inportb (port);

    outportb (port + 1, data);

    for (n = 36; n; n--)
      inportb (port);
  }
}
