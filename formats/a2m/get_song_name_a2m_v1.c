// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_song_name_a2m_v1 (tFIXED_SONGDATA *dst, song_name_a2m_v1_t *src)
{
  CopyString (dst->songname, (String *)src, sizeof (dst->songname) - 1);
}
