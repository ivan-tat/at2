// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_ins_name_mtk (tINSTR_NAME *dst, ins_name_mtk_t *src)
{
  String_t s;

  if (src->length > INSTRUMENT_NAME_LEN_MTK)
    src->length = INSTRUMENT_NAME_LEN_MTK;

  if (src->length > 0)
  {
    String_t t = truncate_string ((String *)src);

    if (t.len >= 10)
      s = Copy ((String *)&t, 10, t.len - 9);
    else
      s.len = 0;
  }
  else s.len = 0;

  AppendString ((String *)dst, (String *)&s, sizeof (*dst) - 1);
}
