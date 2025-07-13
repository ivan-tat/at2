// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void Menu2_refresh (void)
{
  DBG_ENTER ("Menu2_refresh");

  if ((_mnu2.page != _mnu2.opage) || (_mnu2.idx2 != _mnu2.opos) || mn2_environment.do_refresh)
  {
    _mnu2.opage = _mnu2.page;
    _mnu2.opos  = _mnu2.idx2;
    mn2_environment.do_refresh = false;

    for (_mnu2.idx = _mnu2.page; _mnu2.idx <= _mnu2.len2 + _mnu2.page - 1; _mnu2.idx++)
    {
      if (_mnu2.idx == _mnu2.idx2 + _mnu2.page - 1)
      {
        String_t s, t;

        s = Menu2_pstr (_mnu2.idx2 + _mnu2.page - 1);
        t = Menu2_pdes (_mnu2.idx2 + _mnu2.page - 1);
        AppendString ((String *)&s, (String *)&t, 255);
        s = ExpStrR ((String *)&s, _mnu2.max + s.len - CStrLen ((String *)&s), ' ');

        Menu2_ShowCStr (mn2_environment.v_dest,
                        _mnu2.x + 1,
                        _mnu2.y + _mnu2.idx2,
                        (String *)&s,
                        mn2_setting.text2_attr,
                        mn2_setting.short2_attr,
                        mn2_setting.text_attr,
                        mn2_setting.short_attr);
      }
      else
      {
        String_t s, t;
        size_t sl;
        uint8_t attrs[2];

        s = Menu2_pstr (_mnu2.idx);
        sl = s.len;
        t = Menu2_pdes (_mnu2.idx);
        AppendString ((String *)&s, (String *)&t, 255);
        t = Menu2_pdes (_mnu2.idx2 + _mnu2.page - 1);
        s = ExpStrR ((String *)&s, _mnu2.max + sl + t.len - CStrLen ((String *)&s), ' ');

        if (_mnu2.mbuf[_mnu2.idx - 1].use)
        {
          attrs[0] = mn2_setting.text_attr;
          attrs[1] = mn2_setting.short_attr;
        }
        else
        {
          attrs[0] = mn2_setting.disbld_attr;
          attrs[1] = mn2_setting.disbld_attr;
        }

        ShowCStr (mn2_environment.v_dest,
                  _mnu2.x + 1,
                  _mnu2.y + _mnu2.idx - _mnu2.page + 1,
                  (String *)&s,
                  attrs[0],
                  attrs[1]);
      }
    }

    _mnu2.vscrollbar_pos =
      VScrollBar (mn2_environment.v_dest,
                  _mnu2.x + _mnu2.max + 1,
                  _mnu2.y + 1,
                  _mnu2.vscrollbar_size,
                  _mnu2.count,
                  _mnu2.idx2 + _mnu2.page - 1,
                  _mnu2.vscrollbar_pos,
                  mn2_setting.menu_attr,
                  mn2_setting.menu_attr);
  }

  DBG_LEAVE (); //EXIT //Menu2_refresh
}
