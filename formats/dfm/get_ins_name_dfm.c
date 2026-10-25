// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_ins_name_dfm (tINSTR_NAME *dst, ins_name_dfm_t *src)
{
  String_t s;

  if (src->length > INSTRUMENT_NAME_LEN_DFM)
    src->length = INSTRUMENT_NAME_LEN_DFM;

  while (   (src->length != 0)
         && (src->data[src->length - 1] < 32))
    src->length--;

  if (Length (*dst) > 9) SetLength (*dst, 9); // skip first 9 characters
  s = CutStr ((String *)src);
  AppendString ((String *)dst, (String *)&s, sizeof (*dst) - 1);
}
