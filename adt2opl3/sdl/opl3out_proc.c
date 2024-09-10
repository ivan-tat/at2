// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void opl3out_proc (uint16_t reg, uint16_t data) {
  OPL3EMU_WriteReg (reg, data);
}
