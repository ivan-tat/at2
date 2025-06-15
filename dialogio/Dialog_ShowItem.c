// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void Dialog_ShowItem (void)
{
  if (_dlg.idx2 != 0)
  {
    if (_dlg.idx2 != _dlg.idx3)
      ShowCStr (screen_ptr,
                _dlg.dbuf[_dlg.idx3 - 1].pos,
                _dlg.ystart + _dlg.num + 1,
                (String *)&_dlg.dbuf[_dlg.idx3 - 1].str,
                dl_setting.keys_attr,
                dl_setting.short_attr);

    ShowCStr (screen_ptr,
              _dlg.dbuf[_dlg.idx2 - 1].pos,
              _dlg.ystart + _dlg.num + 1,
              (String *)&_dlg.dbuf[_dlg.idx2 - 1].str,
              dl_setting.keys2_attr,
              dl_setting.short2_attr);

    _dlg.idx3 = _dlg.idx2;
  }

  //EXIT //Dialog_ShowItem
}
