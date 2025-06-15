// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// max. length of `src' = 255, max. length of `dst' = 255
static String *_highlight_str (String *dst, const String *src, uint8_t hlight_chrs)
{
  String_t t;

  CopyString (dst, (String *)"\01" "~", 255); // switch color
  t = Copy (src, 1, hlight_chrs);
  AppendString (dst, (String *)&t, 255);
  AppendString (dst, (String *)"\01" "~", 255); // switch color
  t = Copy (src, hlight_chrs + 1, Length (src) - hlight_chrs);
  AppendString (dst, (String *)&t, 255);

  return dst;
}

static void Menu_refresh (void)
{
  DBG_ENTER ("Menu_refresh");

  if ((_mnu.page != _mnu.opage) || (_mnu.idx2 != _mnu.opos) || mn_environment.do_refresh)
  {
    _mnu.opage = _mnu.page;
    _mnu.opos  = _mnu.idx2;
    mn_environment.do_refresh = false;

    if (!mn_environment.own_refresh)
      for (_mnu.idx = _mnu.page; _mnu.idx <= _mnu.len2 + _mnu.page - 1; _mnu.idx++)
      {
        String_t item_str, item_str_alt;
        String_t item_str2, item_str2_alt;
        String_t desc_str, desc_str2, desc_str3;

        item_str      = Menu_pstr (_mnu.idx - _mnu.page + 1);
        item_str_alt  = Menu_pstr (_mnu.idx);
        item_str2     = Menu_pstr2 (_mnu.idx2 + _mnu.page - 1);
        item_str2_alt = Menu_pstr2 (_mnu.idx);
        desc_str  = Menu_pdes (_mnu.idx - _mnu.page + 1);
        desc_str2 = Menu_pdes (_mnu.idx);
        desc_str3 = Menu_pdes (_mnu.idx2 + _mnu.page - 1);

        if (mn_environment.hlight_chrs != 0)
        {
          String_t t;

          if (item_str.len != 0)
            CopyString ((String *)&item_str, _highlight_str ((String *)&t, (String *)&item_str, mn_environment.hlight_chrs), 255);

          if (item_str_alt.len != 0)
            CopyString ((String *)&item_str_alt, _highlight_str ((String *)&t, (String *)&item_str_alt, mn_environment.hlight_chrs), 255);

          if (item_str2.len != 0)
            CopyString ((String *)&item_str2, _highlight_str ((String *)&t, (String *)&item_str2, mn_environment.hlight_chrs), 255);

          if (item_str2_alt.len != 0)
            CopyString ((String *)&item_str2_alt, _highlight_str ((String *)&t, (String *)&item_str2_alt, mn_environment.hlight_chrs), 255);
        }

        if (_mnu.idx == _mnu.idx2 + _mnu.page - 1)
        {
          String_t s;

          CopyString ((String *)&s, (String *)&item_str2, 255);
          AppendString ((String *)&s, (String *)&desc_str3, 255);
          s = ExpStrR ((String *)&s, _mnu.max + item_str2.len + desc_str3.len - C3StrLen ((String *)&item_str2) - CStrLen ((String *)&desc_str3), ' ');
          Menu_ShowCStr (mn_environment.v_dest, _mnu.x + 1, _mnu.y + _mnu.idx2, (String *)&s,
                         mn_setting.text2_attr,
                         mn_setting.short2_attr,
                         mn_setting.text_attr,
                         mn_setting.short_attr);
        }
        else if ((_mnu.idx - _mnu.page + 1) <= _mnu.topic_len)
        {
          String_t s;

          CopyString ((String *)&s, (String *)&item_str, 255);
          AppendString ((String *)&s, (String *)&desc_str, 255);
          s = ExpStrR ((String *)&s, _mnu.max + item_str.len + desc_str3.len - CStrLen ((String *)&s), ' '),
          ShowCStr (mn_environment.v_dest, _mnu.x + 1, _mnu.y + _mnu.idx - _mnu.page + 1, (String *)&s,
                    mn_setting.topic_attr,
                    mn_setting.hi_topic_attr);
        }
        else if (_mnu.mbuf[_mnu.idx - 1].use)
        {
          String_t s;

          CopyString ((String *)&s, (String *)&item_str2_alt, 255);
          AppendString ((String *)&s, (String *)&desc_str2, 255);
          s = ExpStrR ((String *)&s, _mnu.max + item_str2_alt.len + desc_str3.len - C3StrLen ((String *)&item_str2_alt) - CStrLen ((String *)&desc_str2), ' ');
          ShowC3Str (mn_environment.v_dest, _mnu.x + 1, _mnu.y + _mnu.idx - _mnu.page + 1, (String *)&s,
                     mn_setting.text_attr,
                     mn_setting.short_attr,
                     mn_setting.topic_attr);
        }
        else
        {
          String_t s;

          CopyString ((String *)&s, (String *)&item_str_alt, 255);
          AppendString ((String *)&s, (String *)&desc_str2, 255);
          s = ExpStrR ((String *)&s, _mnu.max + item_str_alt.len + desc_str3.len - CStrLen ((String *)&s), ' ');
          ShowCStr (mn_environment.v_dest, _mnu.x + 1, _mnu.y + _mnu.idx - _mnu.page + 1, (String *)&s,
                    mn_setting.disbld_attr,
                    mn_setting.disbld_attr);
        }
      }

    if (mn_setting.show_scrollbar)
      _mnu.vscrollbar_pos =
        VScrollBar (mn_environment.v_dest,
                    _mnu.x + _mnu.max + 1,
                    _mnu.y + 1 - mn_setting.topic_len,
                    _mnu.vscrollbar_size, _mnu.count, _mnu.idx2 + _mnu.page - 1,
                    _mnu.vscrollbar_pos,
                    mn_setting.menu_attr, mn_setting.menu_attr);
  }

  DBG_LEAVE (); //EXIT //Menu_refresh
}
