// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

uint16_t get_4op_to_test (void)
{
  if (songdata.flag_4op != 0)
  {
    if (check_4op_flag (current_inst))
      return current_inst + 1 + (current_inst << 8);
    else if ((current_inst > 1) && check_4op_flag (current_inst - 1))
      return current_inst + ((current_inst - 1) << 8);
  }

  return 0;
}
