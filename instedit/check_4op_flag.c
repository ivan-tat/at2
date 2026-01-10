// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// ins: 1..255
bool check_4op_flag (uint8_t ins)
{
  for (uint_least8_t i = 0; i < songdata.ins_4op_flags.num_4op; i++)
    if (songdata.ins_4op_flags.idx_4op[i] == ins)
      return true;

  return false;
}
