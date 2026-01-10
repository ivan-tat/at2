// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// ins: 1..255
uint16_t check_4op_instrument (uint8_t ins)
{
  if (check_4op_flag (ins))
    return ins + 1 + (ins << 8);
  else if ((ins > 1) && check_4op_flag (ins - 1))
    return ins + ((ins - 1) << 8);

  return 0;
}
