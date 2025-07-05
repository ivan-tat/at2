// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void Menu1_SubPos (uint16_t *p)
{
  uint16_t t = *p;

  if (t > 1)
  {
    t--;
    if (!_mnu1.mbuf[t + _mnu1.page - 1 - 1].use)
      Menu1_SubPos (&t);
  }
  else if (_mnu1.page > _mnu1.first)
    _mnu1.page--;

  if (_mnu1.mbuf[t + _mnu1.page - 1 - 1].use)
    *p = t;
  else if (t + _mnu1.page - 1 > _mnu1.first)
    Menu1_SubPos (&t);
}
