// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void opl2out (uint16_t reg, uint16_t data) {
  // relevant only for DOS version -> option opl_latency=1
  opl3out_proc (reg, data);
}
