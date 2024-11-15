// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

bool is_ins_adsr_data_empty (uint8_t ins) {
  ins--;

  return ((ins_parameter (ins + 1, 5) >> 4) == 0)
      && ((ins_parameter (ins + 1, 4) >> 4) == 0)
      && ((ins_parameter (ins + 1, 5) & 0x0F) == 0)
      && ((ins_parameter (ins + 1, 4) & 0x0F) == 0)
      && ((ins_parameter (ins + 1, 7) >> 4) == 0)
      && ((ins_parameter (ins + 1, 6) >> 4) == 0)
      && ((ins_parameter (ins + 1, 7) & 0x0F) == 0)
      && ((ins_parameter (ins + 1, 6) & 0x0F) == 0);
}
