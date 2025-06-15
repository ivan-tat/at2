// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void Menu_AddPos (uint16_t *p, uint16_t len)
{
  uint16_t t = *p;

  if ((t < len) && (t < _mnu.last) && _mnu.mbuf[t + _mnu.page - 1].use)
    t++;
  else if ((t < len) && (t < _mnu.last))
  {
    t++;
    Menu_AddPos (&t, len);
  }
  else if ((_mnu.page + t) <= _mnu.last)
    _mnu.page++;

  if (_mnu.mbuf[t + _mnu.page - 1 - 1].use)
    *p = t;
  else if ((t + _mnu.page - 1) < _mnu.last)
    Menu_AddPos (&t, len);
}
