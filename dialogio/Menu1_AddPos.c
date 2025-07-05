// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void Menu1_AddPos (uint16_t *p, uint8_t len)
{
  uint16_t t = *p;

  if ((t < len) && (t < _mnu1.last))
  {
    t++;
    if (!_mnu1.mbuf[t + _mnu1.page - 1 - 1].use)
      Menu1_AddPos (&t, len);
  } else if (t + _mnu1.page <= _mnu1.last)
    _mnu1.page++;

  if (_mnu1.mbuf[t + _mnu1.page - 1 - 1].use)
    *p = t;
  else if (t + _mnu1.page - 1 < _mnu1.last)
    Menu1_AddPos (&t, len);
}
