// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void opl3exp (uint16_t data) {
  OPL3EMU_WriteReg ((data & 0xFF) | 0x100, data >> 8);
}
