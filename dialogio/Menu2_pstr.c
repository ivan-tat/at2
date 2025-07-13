// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// item: 1-based
static String_t Menu2_pstr (uint16_t item)
{
  String_t t;

  DBG_ENTER ("Menu2_pstr");

  item--;
  memmove (&t, (uint8_t *)_mnu2.data + item * (_mnu2.len + 1), _mnu2.len + 1);
  t = ExpStrR ((String *)&t, _mnu2.len - (_mnu2.solid ? 0 : 2), ' ');

  DBG_LEAVE (); //EXIT //Menu2_pstr
  return t;
}
