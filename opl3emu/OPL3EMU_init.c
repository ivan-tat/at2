// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void OPL3EMU_init (void) {
  uint8_t i;

  // Initialize slot data
  for (i = 0; i < 36; i++) {
    opl3.slot[i].p_chip = &opl3;
    opl3.slot[i].p_mod = &opl3.out_null;
    opl3.slot[i].eg_rout = 0x1FF;
    opl3.slot[i].eg_out = 0x1FF;
    opl3.slot[i].eg_state = EG_OFF;
    opl3.slot[i].p_trem = (uint8_t *)&opl3.out_null;
  }

  // Initialize chan. data
  for (i = 0; i < 18; i++) {
    opl3.out_l[i] = 0;
    opl3.out_r[i] = 0;
    opl3.chan[i].out_l = WORD_NULL;
    opl3.chan[i].out_r = WORD_NULL;
    opl3.chan[i].p_chip = &opl3;
    opl3.chan[i].ch_type = CH_2OP;
    opl3.chan[i].p_out[0] = &opl3.out_null;
    opl3.chan[i].p_out[1] = &opl3.out_null;
    opl3.chan[i].p_out[2] = &opl3.out_null;
    opl3.chan[i].p_out[3] = &opl3.out_null;
    opl3.chan[i].p_slot[0] = &opl3.slot[CH_SLOT_IDX[i]];
    opl3.chan[i].p_slot[1] = &opl3.slot[CH_SLOT_IDX[i] + 3];
    opl3.slot[CH_SLOT_IDX[i]].p_chan = &opl3.chan[i];
    opl3.slot[CH_SLOT_IDX[i] + 3].p_chan = &opl3.chan[i];

    if (CH_4OP_MASK[i])
      opl3.chan[i].p_chan = &opl3.chan[CH_4OP_MASK[i] - 1];

    chan_set_alg (&opl3.chan[i]);
  }

  // Initialize chip data
  opl3.noise = NOISE_HASH_VAL;
  opl3.timer = 0;
  opl3.nts_bit = 0;
  opl3.dva_bit = 0;
  opl3.dvb_bit = 0;
  opl3.rhy_flag = 0;
  opl3.vib_pos = 0;
  opl3.trem_dir = 0;
  opl3.trem_pos = 0;
  opl3.trem_val = 0;
  opl3.output[0] = 0;
  opl3.output[1] = 0;
}
