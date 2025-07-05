// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void Menu1_refresh (void)
{
  DBG_ENTER ("Menu1_refresh");

  if ((_mnu1.page != _mnu1.opage) || (_mnu1.idx2 != _mnu1.opos) || mn1_environment.do_refresh)
  {
    _mnu1.opage = _mnu1.page;
    _mnu1.opos  = _mnu1.idx2;
    mn1_environment.do_refresh = false;

    for (_mnu1.idx = _mnu1.page; _mnu1.idx <= _mnu1.len2 + _mnu1.page - 1; _mnu1.idx++)
    {
      if (_mnu1.idx == _mnu1.idx2 + _mnu1.page - 1)
      {
        String_t s, t;

        s = Menu1_pstr (_mnu1.idx2 + _mnu1.page - 1);
        t = Menu1_pdes (_mnu1.idx2 + _mnu1.page - 1);
        AppendString ((String *)&s, (String *)&t, 255);
        s = ExpStrR ((String *)&s, _mnu1.max + s.len - CStrLen ((String *)&s), ' ');

        Menu1_ShowCStr (mn1_environment.v_dest,
                        _mnu1.x + 1,
                        _mnu1.y + _mnu1.idx2,
                        (String *)&s,
                        mn1_setting.text2_attr,
                        mn1_setting.short2_attr,
                        mn1_setting.text_attr,
                        mn1_setting.short_attr);
      }
      else
      {
        String_t s, t;
        size_t sl;
        uint8_t attrs[2];

        s = Menu1_pstr (_mnu1.idx);
        sl = s.len;
        t = Menu1_pdes (_mnu1.idx);
        AppendString ((String *)&s, (String *)&t, 255);
        t = Menu1_pdes (_mnu1.idx2 + _mnu1.page - 1);
        s = ExpStrR ((String *)&s, _mnu1.max + sl + t.len - CStrLen ((String *)&s), ' ');

        if (_mnu1.mbuf[_mnu1.idx - 1].use)
        {
          attrs[0] = mn1_setting.text_attr;
          attrs[1] = mn1_setting.short_attr;
        }
        else
        {
          attrs[0] = mn1_setting.disbld_attr;
          attrs[1] = mn1_setting.disbld_attr;
        }

        ShowCStr (mn1_environment.v_dest,
                  _mnu1.x + 1,
                  _mnu1.y + _mnu1.idx - _mnu1.page + 1,
                  (String *)&s,
                  attrs[0],
                  attrs[1]);
      }
    }

    _mnu1.vscrollbar_pos =
      VScrollBar (mn1_environment.v_dest,
                  _mnu1.x + _mnu1.max + 1,
                  _mnu1.y + 1,
                  _mnu1.vscrollbar_size,
                  _mnu1.count,
                  _mnu1.idx2 + _mnu1.page - 1,
                  _mnu1.vscrollbar_pos,
                  mn1_setting.menu_attr,
                  mn1_setting.menu_attr);
  }

  DBG_LEAVE (); //EXIT //Menu1_refresh
}
