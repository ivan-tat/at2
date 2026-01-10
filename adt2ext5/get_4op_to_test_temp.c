// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*static*/ uint16_t get_4op_to_test_temp (void)
{
  uint16_t result = 0;
  int_least16_t n = mn_environment.curr_pos - 1; // current instrument index

  if ((n >= 0) && (n <= 254) && (songdata.flag_4op != 0))
  {
    if (_4op_flag_column[n] == charmap.four_op_upper_half)
      result = n + 1 + 1 + ((n + 1) << 8);
    else if ((n > 0) && (_4op_flag_column[n] == charmap.four_op_lower_half))
      result = n + 1 + ((n - 1 + 1) << 8);
  }
  return result;
}
