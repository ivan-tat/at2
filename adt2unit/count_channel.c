// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

uint8_t count_channel (uint8_t hpos)
{
  uint8_t a = _pattedit_lastpos / MAX_TRACKS;

  return chan_pos + hpos / a - ((!(hpos % a)) ? 1 : 0);
}
