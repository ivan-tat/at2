// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// ins: 1-based
uint8_t ins_parameter (uint8_t ins, uint8_t param) {
  return *((uint8_t *)songdata.instr_data + (ins - 1) * INSTRUMENT_SIZE + param);
}
