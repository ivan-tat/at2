// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void Menu_SubPos (uint16_t *p)
{
  uint16_t t = *p;

  if ((t > 1) && _mnu.mbuf[t + _mnu.page - 2 - 1].use)
    t--;
  else if (t > 1)
  {
    t--;
    Menu_SubPos (&t);
  }
  else if (_mnu.page > _mnu.first)
    _mnu.page--;

  if (_mnu.mbuf[t + _mnu.page - 1 - 1].use)
    *p = t;
  else if (t + _mnu.page - 1 > _mnu.first)
    Menu_SubPos (&t);
}
