// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void chan_set_alg (OPL3_CHAN *p_chan) {
  switch (p_chan->ch_type) {
  case CH_2OP:
    switch (p_chan->alg & 1) {
    case 0: // FM
      p_chan->p_slot[0]->p_mod = &p_chan->p_slot[0]->fb_out;
      p_chan->p_slot[1]->p_mod = &p_chan->p_slot[0]->output;
      p_chan->p_out[0] = &p_chan->p_slot[1]->output;
      p_chan->p_out[1] = &p_chan->p_chip->out_null;
      p_chan->p_out[2] = &p_chan->p_chip->out_null;
      p_chan->p_out[3] = &p_chan->p_chip->out_null;
      break;

    case 1: // AM
      p_chan->p_slot[0]->p_mod = &p_chan->p_slot[0]->fb_out;
      p_chan->p_slot[1]->p_mod = &p_chan->p_chip->out_null;
      p_chan->p_out[0] = &p_chan->p_slot[0]->output;
      p_chan->p_out[1] = &p_chan->p_slot[1]->output;
      p_chan->p_out[2] = &p_chan->p_chip->out_null;
      p_chan->p_out[3] = &p_chan->p_chip->out_null;
      break;

    default:
      break;
    }
    break; // CH_2OP

  case CH_4OP_2:
    p_chan->p_chan->p_out[0] = &p_chan->p_chip->out_null;
    p_chan->p_chan->p_out[1] = &p_chan->p_chip->out_null;
    p_chan->p_chan->p_out[2] = &p_chan->p_chip->out_null;
    p_chan->p_chan->p_out[3] = &p_chan->p_chip->out_null;

    switch (p_chan->alg & 3) {
    case 0: // FM-FM
      p_chan->p_chan->p_slot[0]->p_mod = &p_chan->p_chan->p_slot[0]->fb_out;
      p_chan->p_chan->p_slot[1]->p_mod = &p_chan->p_chan->p_slot[0]->output;
      p_chan->p_slot[0]->p_mod = &p_chan->p_chan->p_slot[1]->output;
      p_chan->p_slot[1]->p_mod = &p_chan->p_slot[0]->output;
      p_chan->p_out[0] = &p_chan->p_slot[1]->output;
      p_chan->p_out[1] = &p_chan->p_chip->out_null;
      p_chan->p_out[2] = &p_chan->p_chip->out_null;
      p_chan->p_out[3] = &p_chan->p_chip->out_null;
      break;

    case 1: // FM-AM
      p_chan->p_chan->p_slot[0]->p_mod = &p_chan->p_chan->p_slot[0]->fb_out;
      p_chan->p_chan->p_slot[1]->p_mod = &p_chan->p_chan->p_slot[0]->output;
      p_chan->p_slot[0]->p_mod = &p_chan->p_chip->out_null;
      p_chan->p_slot[1]->p_mod = &p_chan->p_slot[0]->output;
      p_chan->p_out[0] = &p_chan->p_chan->p_slot[1]->output;
      p_chan->p_out[1] = &p_chan->p_slot[1]->output;
      p_chan->p_out[2] = &p_chan->p_chip->out_null;
      p_chan->p_out[3] = &p_chan->p_chip->out_null;
      break;

    case 2: // AM-AM
      p_chan->p_chan->p_slot[0]->p_mod = &p_chan->p_chan->p_slot[0]->fb_out;
      p_chan->p_chan->p_slot[1]->p_mod = &p_chan->p_chip->out_null;
      p_chan->p_slot[0]->p_mod = &p_chan->p_chan->p_slot[1]->output;
      p_chan->p_slot[1]->p_mod = &p_chan->p_slot[0]->output;
      p_chan->p_out[0] = &p_chan->p_chan->p_slot[0]->output;
      p_chan->p_out[1] = &p_chan->p_slot[1]->output;
      p_chan->p_out[2] = &p_chan->p_chip->out_null;
      p_chan->p_out[3] = &p_chan->p_chip->out_null;
      break;

    case 3: // AM-FM
      p_chan->p_chan->p_slot[0]->p_mod = &p_chan->p_chan->p_slot[0]->fb_out;
      p_chan->p_chan->p_slot[1]->p_mod = &p_chan->p_chip->out_null;
      p_chan->p_slot[0]->p_mod = &p_chan->p_chan->p_slot[1]->output;
      p_chan->p_slot[1]->p_mod = &p_chan->p_chip->out_null;
      p_chan->p_out[0] = &p_chan->p_chan->p_slot[0]->output;
      p_chan->p_out[1] = &p_chan->p_slot[0]->output;
      p_chan->p_out[2] = &p_chan->p_slot[1]->output;
      p_chan->p_out[3] = &p_chan->p_chip->out_null;
      break;

    default:
      break;
    }
    break; // CH_4OP_2

  case CH_RHYTHM:
    switch (p_chan->alg & 1) {
    case 0: // FM
      p_chan->p_slot[0]->p_mod = &p_chan->p_slot[0]->fb_out;
      p_chan->p_slot[1]->p_mod = &p_chan->p_slot[0]->output;
      break;

    case 1: // AM
      p_chan->p_slot[0]->p_mod = &p_chan->p_slot[0]->fb_out;
      p_chan->p_slot[1]->p_mod = &p_chan->p_chip->out_null;
      break;

    default:
      break;
    }
    break; // CH_RHYTHM

  default:
    break;
  }
}
