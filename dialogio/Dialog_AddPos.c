// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void Dialog_AddPos (uint16_t *p)
{
  uint16_t t = *p;

  if ((t < _dlg.nm2) && _dlg.dbuf[t + 1 - 1].use)
    t++;
  else if (t < _dlg.nm2)
  {
    t++;
    Dialog_AddPos (&t);
  }
  if (_dlg.dbuf[t - 1].use)
    *p = t;
}
