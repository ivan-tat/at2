// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void Menu2_SubPos (uint16_t *p)
{
  uint16_t t = *p;

  if (t > 1)
  {
    t--;
    if (!_mnu2.mbuf[t + _mnu2.page - 1 - 1].use)
      Menu2_SubPos (&t);
  }
  else if (_mnu2.page > _mnu2.first)
    _mnu2.page--;

  if (_mnu2.mbuf[t + _mnu2.page - 1 - 1].use)
    *p = t;
  else if (t + _mnu2.page - 1 > _mnu2.first)
    Menu2_SubPos (&t);
}
