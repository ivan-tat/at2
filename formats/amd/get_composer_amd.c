// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_composer_amd (tFIXED_SONGDATA *dst, composer_amd_t *src)
{
  String_t s, t;

  StrToString ((String *)&s, (char *)src, sizeof (*src));
  t = asciiz_string ((String *)&s);
  s = CutStr ((String *)&t);
  CopyString (dst->composer, (String *)&s, sizeof (dst->composer) - 1);
}
