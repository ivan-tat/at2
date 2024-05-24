// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void update_fb_con (OPL3_CHAN *p_chan, uint8_t data) {
  p_chan->fb = (data & 0x0E) >> 1;
  p_chan->con = data & 1;
  p_chan->alg = p_chan->con;

  switch (p_chan->ch_type) {
  case CH_2OP:
  case CH_RHYTHM:
    chan_set_alg (p_chan);
    break;

  case CH_4OP_1:
    p_chan->p_chan->alg = 4
                        | (p_chan->con << 1)
                        | (p_chan->p_chan->con);
    chan_set_alg (p_chan->p_chan);
    break;

  case CH_4OP_2:
    p_chan->alg = 4
                | (p_chan->p_chan->con << 1)
                | (p_chan->con);
    chan_set_alg (p_chan);
    break;

  default:
    break;
  }

  // Trigger output to left
  p_chan->out_l = ((data >> 4) & 1) ? WORD_NULL : 0;

  // Trigger output to right
  p_chan->out_r = ((data >> 5) & 1) ? WORD_NULL : 0;
}
