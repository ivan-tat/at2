// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void reset_marked_instruments (void)
{
  if (marked_instruments () < 2)
  {
    unsigned char *c;

    c = &(GetStr (songdata.instr_names[instrum_page - 1])[0]);
    if (*c == _INS_MARK_CHR)
      *c = ' ';
  }
  else
  {
    GetStr (songdata.instr_names[_2nd_marked () - 1])[0] = ' ';
    GetStr (songdata.instr_names[_1st_marked () - 1])[0] = ' ';
  }
  update_4op_flag_marks ();
}
