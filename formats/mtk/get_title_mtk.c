// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_title_mtk (tFIXED_SONGDATA *dst, title_mtk_t *src)
{
  String_t s;

  if (src->length > TITLE_LEN_MTK)
    src->length = TITLE_LEN_MTK;

  s = CutStr ((String *)src);
  CopyString (dst->songname, (String *)&s, sizeof (dst->songname) - 1);
}
