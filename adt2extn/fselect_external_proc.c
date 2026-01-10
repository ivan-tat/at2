// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*static*/ void fselect_external_proc (void)
{
  uint8_t ins1, ins2;

  DBG_ENTER ("fselect_external_proc");

  if (songdata.flag_4op != 0)
  {
    ins1 = current_inst < 255 ? current_inst : (current_inst - 1);
    ins2 = ins1 + 1;
  }
  else
  {
    ins1 = current_inst;
    ins2 = 0;
  }
  if (!shift_pressed () && !alt_pressed () && !ctrl_pressed ())
    test_instrument_alt (count_channel (pattern_hpos),
                         mn_environment.keystroke,
                         false, false, false,
                         ins1, ins2);

  DBG_LEAVE (); //EXIT //fselect_external_proc
}
