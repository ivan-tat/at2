// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// item: 1-based
static String_t Menu_pstr (uint16_t item)
{
  String_t t;

  DBG_ENTER ("Menu_pstr");

  item--;
  if (item < _mnu.count)
    memmove (&t, (uint8_t *)_mnu.data + item * (_mnu.len + 1), _mnu.len + 1);
  else
    t.len = 0;

  t = ExpStrR ((String *)&t, !_mnu.solid ? _mnu.len - 2 : _mnu.len, ' ');

  DBG_LEAVE (); //EXIT //Menu_pstr
  return t;
}
