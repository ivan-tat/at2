// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void chan_update_rhythm (OPL3_CHIP *p_chip, uint8_t data) {
  p_chip->rhy_flag = data & 0x3F;
  if (p_chip->rhy_flag & 0x20) {
      // BD
      p_chip->chan[6].ch_type = CH_RHYTHM;
      p_chip->chan[6].p_out[0] = &p_chip->chan[6].p_slot[1]->output;
      p_chip->chan[6].p_out[1] = &p_chip->chan[6].p_slot[1]->output;
      p_chip->chan[6].p_out[2] = &p_chip->out_null;
      p_chip->chan[6].p_out[3] = &p_chip->out_null;

      chan_set_alg (&p_chip->chan[6]);
      eg_key_on_off (p_chip->chan[6].p_slot[0], (p_chip->rhy_flag & 0x10) != 0);
      eg_key_on_off (p_chip->chan[6].p_slot[1], (p_chip->rhy_flag & 0x10) != 0);

      // HH + SD
      p_chip->chan[7].ch_type = CH_RHYTHM;
      p_chip->chan[7].p_out[0] = &p_chip->chan[7].p_slot[0]->output;
      p_chip->chan[7].p_out[1] = &p_chip->chan[7].p_slot[0]->output;
      p_chip->chan[7].p_out[2] = &p_chip->chan[7].p_slot[1]->output;
      p_chip->chan[7].p_out[3] = &p_chip->chan[7].p_slot[1]->output;

      eg_key_on_off (p_chip->chan[7].p_slot[0], (p_chip->rhy_flag & 0x01) != 0);
      eg_key_on_off (p_chip->chan[7].p_slot[1], (p_chip->rhy_flag & 0x08) != 0);

      // TT + TC
      p_chip->chan[8].ch_type = CH_RHYTHM;
      p_chip->chan[8].p_out[0] = &p_chip->chan[8].p_slot[0]->output;
      p_chip->chan[8].p_out[1] = &p_chip->chan[8].p_slot[0]->output;
      p_chip->chan[8].p_out[2] = &p_chip->chan[8].p_slot[1]->output;
      p_chip->chan[8].p_out[3] = &p_chip->chan[8].p_slot[1]->output;

      eg_key_on_off (p_chip->chan[8].p_slot[0], (p_chip->rhy_flag & 0x04) != 0);
      eg_key_on_off (p_chip->chan[8].p_slot[1], (p_chip->rhy_flag & 0x02) != 0);
  } else {
    // Reset chan. 6/7/8 to 2OP
    p_chip->chan[6].ch_type = CH_2OP;
    chan_set_alg (&p_chip->chan[6]);
    p_chip->chan[7].ch_type = CH_2OP;
    chan_set_alg (&p_chip->chan[7]);
    p_chip->chan[8].ch_type = CH_2OP;
    chan_set_alg (&p_chip->chan[8]);
  }
}
