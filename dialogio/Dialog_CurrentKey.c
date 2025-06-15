// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static uint8_t Dialog_CurrentKey (uint8_t pos)
{
  uint8_t r = 0;

  for (uint8_t i = 0; i < _dlg.nm2; i++)
    if (   (pos ==  _dlg.dbuf[i].pos)
        || (pos == (_dlg.dbuf[i].pos + CStrLen ((String *)&_dlg.dbuf[i].str) - 1)))
      r = i + 1;

  return r;
}
