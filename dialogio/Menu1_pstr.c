// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// item: 1-based
static String_t Menu1_pstr (uint16_t item)
{
  String_t t;

  DBG_ENTER ("Menu1_pstr");

  item--;
  memmove (&t, (uint8_t *)_mnu1.data + item * (_mnu1.len + 1), _mnu1.len + 1);
  t = ExpStrR ((String *)&t, _mnu1.len - (_mnu1.solid ? 0 : 2), ' ');

  DBG_LEAVE (); //EXIT //Menu1_pstr
  return t;
}
