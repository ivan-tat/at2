// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// item: 1-based
static String_t Menu_pstr2 (uint16_t item)
{
  String_t t, r;

  if (item <= _mnu.count)
    memmove (&t, (uint8_t *)_mnu.data + (item - 1) * (_mnu.len + 1), _mnu.len + 1);
  else
    t.len = 0;

  t = ExpStrR ((String *)&t, !_mnu.solid ? _mnu.len - 2 : _mnu.len, ' ');

  if (mn_setting.fixed_len != 0)
    CopyString ((String *)&r, (String *)&t, 255);
  else
  {
    r.len = 0;
    for (uint8_t i = 0; i < t.len; i++)
      if (bit_test ((const uint8_t *)&mn_setting.topic_mask_chr, t.str[i]))
      {
        r.str[r.len + 0] = '`';
        r.str[r.len + 1] = t.str[i];
        r.str[r.len + 2] = '`';
        r.len += 3;
      }
      else
      {
        r.str[r.len] = t.str[i];
        r.len++;
      }
  }

  return r;
}
