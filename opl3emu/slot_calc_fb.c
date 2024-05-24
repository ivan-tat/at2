// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void slot_calc_fb (OPL3_SLOT *p_slot) {
  p_slot->fb_out = p_slot->p_chan->fb ? (p_slot->prev_out + p_slot->output)
                                        >> (9 - p_slot->p_chan->fb)
                                      : 0;
  p_slot->prev_out = p_slot->output;
}
