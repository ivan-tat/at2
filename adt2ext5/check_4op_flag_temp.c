// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// ins: 1..255
/*static*/ bool check_4op_flag_temp (uint8_t ins)
{
  for (uint8_t idx = 0; idx < temp_songdata.ins_4op_flags.num_4op; idx++)
    if (temp_songdata.ins_4op_flags.idx_4op[idx] == ins)
      return true;

  return false;
}
