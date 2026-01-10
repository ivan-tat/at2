// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*static*/ void a2b_lister_external_proc_callback (void)
{
  uint16_t test_4op_ins;
  uint8_t test_ins1, test_ins2;

  DBG_ENTER ("a2b_lister_external_proc_callback");

  if ((test_4op_ins = get_4op_to_test_temp ()) != 0)
  {
    test_ins1 = test_4op_ins & 0xFF;
    test_ins2 = test_4op_ins >> 8;
  }
  else
  {
    uint8_t curr_inst = mn_environment.curr_pos;

    if (curr_inst == 0) goto _exit;

    test_ins1 = curr_inst;
    test_ins2 = 0;
  }

  if (!shift_pressed () && !alt_pressed () && !ctrl_pressed ())
    test_instrument_alt (count_channel (pattern_hpos),
                         mn_environment.keystroke,
                         false, true, false, // test instrument from bank, without any macros
                         test_ins1, test_ins2);

_exit:
  DBG_LEAVE (); //EXIT //a2b_lister_external_proc_callback
}
