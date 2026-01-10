// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// ins: 1..255
void reset_4op_flag (uint8_t ins)
{
  ins--; // ins: 0..254
  if (check_4op_flag (ins + 1))
  {
    tINS_4OP_FLAGS t;
    uint_least8_t i;

    t.num_4op = 0;

    for (i = 0;    (i < songdata.ins_4op_flags.num_4op)
                && (songdata.ins_4op_flags.idx_4op[i] <= ins); i++)
      t.idx_4op[t.num_4op++] = songdata.ins_4op_flags.idx_4op[i];

    for (i++; i < songdata.ins_4op_flags.num_4op; i++)
      t.idx_4op[t.num_4op++] = songdata.ins_4op_flags.idx_4op[i];

    songdata.ins_4op_flags = t;
  }
}
