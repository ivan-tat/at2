// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void bnk_lister_external_proc (void)
{
  char s[31+1];

  DBG_ENTER ("bnk_lister_external_proc");

  snprintf (s, sizeof (s), "%d.bnk", mn_environment.curr_pos);
  StrToString (mn_environment.curr_item, s, sizeof (mn_environment.curr_item) - 1);

  if (!shift_pressed () && !alt_pressed () && !ctrl_pressed ())
    test_instrument_alt2 (&temp_instrument, count_channel (pattern_hpos),
                          mn_environment.keystroke);

  DBG_LEAVE (); //EXIT //bnk_lister_external_proc
}
