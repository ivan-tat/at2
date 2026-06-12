// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_ins_name_cff (tINSTR_NAME *dst, ins_name_cff_t *src)
{
  String_t s, t;

  for (unsigned i = 0; i < sizeof (*src); i++)
    if ((*src)[i] == '\0') (*src)[i] = ' ';

  t = truncate_string (StrToString ((String *)&s, *src, sizeof (*src)));
  if (Length (*dst) > 9) SetLength (*dst, 9); // skip first 9 characters
  AppendString ((String *)dst, (String *)&t, sizeof (*dst) - 1);
}
