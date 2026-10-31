// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_ins_name_fmk (tINSTR_NAME *dst, ins_data_fmk_t *src)
{
  String_t s;

  if (Length (*dst) > 9) SetLength (*dst, 9); // skip first 9 characters
  s = truncate_string (StrToString ((String *)&s, src->ins_name, sizeof (src->ins_name)/*-1*/));
  if (s.len == 0)
    s = truncate_string (StrToString ((String *)&s, src->dos_name, sizeof (src->dos_name)/*-1*/));
  AppendString ((String *)dst, (String *)&s, sizeof (*dst) - 1);
}
