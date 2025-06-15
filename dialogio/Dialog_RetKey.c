// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static uint16_t Dialog_RetKey (uint8_t code)
{
  uint8_t uc = UpCase (code);

  for (uint8_t i = 0; i < _dlg.nm2; i++)
    if (UpCase (_dlg.dbuf[i].key) == uc)
      return i + 1;

  return 0;
}
