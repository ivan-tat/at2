// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// item: 1-based
static String_t Menu_pdes (uint16_t item)
{
  String_t t;

  DBG_ENTER ("Menu_pdes");

  if ((mn_environment.descr != NULL) && (item <= _mnu.count))
    memmove (&t, (uint8_t *)mn_environment.descr + (item - 1) * (mn_environment.descr_len + 1), mn_environment.descr_len + 1);
  else
    t.len = 0;

  t = ExpStrR ((String *)&t, mn_environment.descr_len, ' ');

  DBG_LEAVE (); //EXIT //Menu_pdes
  return t;
}
