// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void update_4op_flag_marks (void)
{
  for (uint_least8_t i = 0; i < 254; i++)
    if (!check_4op_flag (i + 1))
    {
      unsigned char *c;

      c = &(GetStr (songdata.instr_names[i])[0]);
      if ((*c == charmap.four_op_upper_half) || (*c == charmap.four_op_lower_half))
        *c = ' ';
      c = &(GetStr (songdata.instr_names[i + 1])[0]);
      if ((*c == charmap.four_op_upper_half) || (*c == charmap.four_op_lower_half))
        *c = ' ';
    }

  for (uint_least8_t i = 0; i < 254; i++)
    if (check_4op_flag (i + 1))
    {
      unsigned char *c;

      c = &(GetStr (songdata.instr_names[i])[0]);
      if (*c == ' ')
        *c = charmap.four_op_upper_half;
      c = &(GetStr (songdata.instr_names[i + 1])[0]);
      if (*c == ' ')
        *c = charmap.four_op_lower_half;
    }
}
