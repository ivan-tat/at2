// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void ibk_lister_external_proc (void)
{
  char s[31+1];

  DBG_ENTER ("ibk_lister_external_proc");

  snprintf (s, sizeof (s), "%d.ibk", mn_environment.curr_pos);
  StrToString (mn_environment.curr_item, s, sizeof (mn_environment.curr_item) - 1);

  if (!shift_pressed () && !alt_pressed () && !ctrl_pressed ())
    test_instrument_alt2 (&temp_instrument, count_channel (pattern_hpos),
                          mn_environment.keystroke);

  DBG_LEAVE (); //EXIT //ibk_lister_external_proc
}
