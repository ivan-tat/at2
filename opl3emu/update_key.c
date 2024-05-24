// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void update_key (OPL3_CHAN *p_chan, bool key_on) {
  switch (p_chan->ch_type) {
  case CH_2OP:
  case CH_4OP_1:
  case CH_RHYTHM:
    eg_key_on_off (p_chan->p_slot[0], key_on);
    eg_key_on_off (p_chan->p_slot[1], key_on);

    if (p_chan->ch_type == CH_4OP_1) {
      eg_key_on_off (p_chan->p_chan->p_slot[0], key_on);
      eg_key_on_off (p_chan->p_chan->p_slot[1], key_on);
    }
    break;

  default:
    break;
  }
}
