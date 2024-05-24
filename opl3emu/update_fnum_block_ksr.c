// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void update_fnum_block_ksr (OPL3_CHAN *p_chan, uint8_t data,
                                   bool msb_flag) {

  if (p_chan->ch_type == CH_4OP_2)
    return;

  if (msb_flag) {
    // Update upper bits
    p_chan->fnum = (p_chan->fnum & 0x300) | data;
    p_chan->ksr = (p_chan->block << 1)
                | ((p_chan->fnum >> (9 - p_chan->p_chip->nts_bit)) & 1);
  } else {
    // Update lower bits
    p_chan->fnum = (p_chan->fnum & 0x0FF) | ((data & 3) << 8);
    p_chan->block = (data >> 2) & 7;
    p_chan->ksr = (p_chan->block << 1)
                | ((p_chan->fnum >> (9 - p_chan->p_chip->nts_bit)) & 1);
  }

  envelope_update_ksl (p_chan->p_slot[0]);
  envelope_update_ksl (p_chan->p_slot[1]);
  envelope_update_rate (p_chan->p_slot[0]);
  envelope_update_rate (p_chan->p_slot[1]);

  if (p_chan->ch_type == CH_4OP_1) {
      p_chan->p_chan->fnum = p_chan->fnum;
      p_chan->p_chan->ksr = p_chan->ksr;

      if (msb_flag)
        p_chan->p_chan->block = p_chan->block;

      envelope_update_ksl (p_chan->p_chan->p_slot[0]);
      envelope_update_ksl (p_chan->p_chan->p_slot[1]);
      envelope_update_rate (p_chan->p_chan->p_slot[0]);
      envelope_update_rate (p_chan->p_chan->p_slot[1]);
  }
}
