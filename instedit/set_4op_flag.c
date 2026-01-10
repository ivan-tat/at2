// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// ins: 1..255
void set_4op_flag (uint8_t ins)
{
  ins--; // ins: 0..254
  if (ins != 254)
  {
    tINS_4OP_FLAGS t;
    uint_least8_t i;

    if (check_4op_flag (ins + 1))
      reset_4op_flag (ins + 1);
    if ((ins > 0) && check_4op_flag (ins + 1 - 1))
      reset_4op_flag (ins + 1 - 1);
    if ((ins < 254) && check_4op_flag (ins + 1 + 1))
      reset_4op_flag (ins + 1 + 1);

    t.num_4op = 0;
    for (i = 0;    (i < songdata.ins_4op_flags.num_4op)
                && (songdata.ins_4op_flags.idx_4op[i] <= ins); i++)
      t.idx_4op[t.num_4op++] = songdata.ins_4op_flags.idx_4op[i];

    t.idx_4op[t.num_4op++] = ins + 1;
    for (; i < songdata.ins_4op_flags.num_4op; i++)
      t.idx_4op[t.num_4op++] = songdata.ins_4op_flags.idx_4op[i];

    songdata.ins_4op_flags = t;
  }

  //EXIT //set_4op_flag
}
