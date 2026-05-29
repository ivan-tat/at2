// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void apply_song_flags (tFIXED_SONGDATA *song)
{
  uint8_t m_or  = 0;
  uint8_t m_and = 0xFF;

  if (song->common_flag & (1 << 1)) m_or  |= 0x10;
  if (song->common_flag & (1 << 2)) m_or  |= 0x20;
  if (song->common_flag & (1 << 5)) m_and &= ~0x03;

  if ((m_or != 0) || (m_and != 0xFF))
    for (uint8_t i = 0; i < CHANNELS_MAX; i++)
      song->lock_flags[i] = (song->lock_flags[i] | m_or) & m_and;
}
