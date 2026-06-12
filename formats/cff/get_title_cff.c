// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_title_cff (tFIXED_SONGDATA *dst, title_cff_t *src)
{
  String_t s, t;

  t = truncate_string (StrToString ((String *)&s, *src, sizeof (*src)));
  CopyString (dst->songname, (String *)&t, sizeof (dst->songname) - 1);
}
