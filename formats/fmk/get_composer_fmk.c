// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_composer_fmk (tFIXED_SONGDATA *dst, composer_fmk_t *src)
{
  String_t s, t;

  t = truncate_string (StrToString ((String *)&s, *src, sizeof (*src)/*-1*/));
  CopyString (dst->composer, (String *)&t, sizeof (dst->composer) - 1);
}
