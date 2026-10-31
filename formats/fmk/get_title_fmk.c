// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_title_fmk (tFIXED_SONGDATA *dst, title_fmk_t *src)
{
  String_t s;

  s = truncate_string (StrToString ((String *)&s, *src, sizeof (*src)/*-1*/));
  CopyString (dst->songname, (String *)&s, sizeof (dst->songname) - 1);
}
