// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*static*/ void reset_instrument_preview (void)
{
  uint16_t ins;

  if (play_status != isPlaying) reset_player ();
  ins = check_4op_to_test ();
  if (ins == 0)
    update_instr_data (instrum_page);
  else
  {
    update_instr_data (ins & 0xFF);
    update_instr_data (ins >> 8);
  }
}
