// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void Menu2_AddPos (uint16_t *p, uint8_t len)
{
  uint16_t t = *p;

  if ((t < len) && (t < _mnu2.last))
  {
    t++;
    if (!_mnu2.mbuf[t + _mnu2.page - 1 - 1].use)
      Menu2_AddPos (&t, len);
  } else if (t + _mnu2.page <= _mnu2.last)
    _mnu2.page++;

  if (_mnu2.mbuf[t + _mnu2.page - 1 - 1].use)
    *p = t;
  else if (t + _mnu2.page - 1 < _mnu2.last)
    Menu2_AddPos (&t, len);
}
