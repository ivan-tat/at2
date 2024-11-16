// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void init_player (void)
{
  DBG_ENTER ("init_player");

#if !ADT2PLAY
#if !GO32
  opl3_init ();
#endif // !GO32

  memset (ai_table, 0, sizeof (ai_table));
#endif // !ADT2PLAY

  opl2out (0x01, 0);

  for (uint8_t i = 0; i < 18; i++)
    opl2out (0xB0 + _chan_n[i], 0);

  for (uint8_t i = 0x80; i <= 0x8D; i++)
    opl2out (i, BYTE_NULL);

  for (uint8_t i = 0x90; i <= 0x95; i++)
    opl2out (i, BYTE_NULL);

#if ADT2PLAY
  speed_update    = (songdata.common_flag >> 0) & 1;
  lockvol         = (songdata.common_flag >> 1) & 1;
  lockVP          = (songdata.common_flag >> 2) & 1;
  tremolo_depth   = (songdata.common_flag >> 3) & 1;
  vibrato_depth   = (songdata.common_flag >> 4) & 1;
  panlock         = (songdata.common_flag >> 5) & 1;
  percussion_mode = (songdata.common_flag >> 6) & 1;
  volume_scaling  = (songdata.common_flag >> 7) & 1;
#endif // ADT2PLAY

  current_tremolo_depth = tremolo_depth;
  current_vibrato_depth = vibrato_depth;
#if ADT2PLAY
  init_buffers ();

  if (!percussion_mode)
  {
    memcpy (_chan_n, _chmm_n, sizeof (_chan_n));
    memcpy (_chan_m, _chmm_m, sizeof (_chan_m));
    memcpy (_chan_c, _chmm_c, sizeof (_chan_c));
  }
  else
  {
    memcpy (_chan_n, _chpm_n, sizeof (_chan_n));
    memcpy (_chan_m, _chpm_m, sizeof (_chan_m));
    memcpy (_chan_c, _chpm_c, sizeof (_chan_c));
  }
#endif // ADT2PLAY

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
#if !ADT2PLAY
  init_buffers ();
#endif // !ADT2PLAY

  global_volume = 63;
#if !ADT2PLAY
  macro_ticklooper = 0;
#endif // !ADT2PLAY
  vibtrem_speed_factor = def_vibtrem_speed_factor;
  vibtrem_table_size = def_vibtrem_table_size;
  memmove (vibtrem_table, def_vibtrem_table, sizeof (vibtrem_table));

  for (uint8_t i = 0; i < 20; i++)
  {
    arpgg_table[i].state = 1;
#if !ADT2PLAY
    arpgg_table2[i].state = 1;
#endif // !ADT2PLAY
    voice_table[i] = i + 1;
  }

  DBG_LEAVE (); //EXIT //init_player
}
