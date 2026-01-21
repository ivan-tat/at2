// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*static*/ void import_old_flags (void)
{
  tFIXED_SONGDATA *song = &songdata;

  if ((song->common_flag & 0x02) != 0)
    for (uint8_t i = 0; i < 20; i++) song->lock_flags[i] |= 0x10;

  if ((song->common_flag & 0x04) != 0)
    for (uint8_t i = 0; i < 20; i++) song->lock_flags[i] |= 0x20;

  if ((song->common_flag & 0x20) != 0)
    for (uint8_t i = 0; i < 20; i++) song->lock_flags[i] &= ~0x03;
}
