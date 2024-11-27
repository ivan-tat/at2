// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void reset_player (void)
{
  DBG_ENTER ("reset_player");

  opl2out (0x01, 0);

  for (uint8_t chan = 0; chan <= 17; chan++)
    opl2out (0xB0 + _chan_n[chan], 0);

  for (uint8_t reg = 0x80; reg <= 0x8D; reg++)
    opl2out (reg, BYTE_NULL);

  for (uint8_t reg = 0x90; reg <= 0x95; reg++)
    opl2out (reg, BYTE_NULL);

  misc_register =   (tremolo_depth << 7)
                  + (vibrato_depth << 6)
                  + (percussion_mode ? 1 << 5 : 0);

  opl2out (0x01, 0x20);
  opl2out (0x08, 0x40);
  opl3exp (0x0105);
  opl3exp (0x04 + (songdata.flag_4op << 8));

  key_off (16 + 1);
  key_off (17 + 1);
  opl2out (_instr[11], misc_register);

  for (uint8_t chan = 0; chan <= 19; chan++)
    reset_chan_data (chan + 1);

  DBG_LEAVE (); //EXIT //reset_player
}
