// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// spos: 1-based
uint16_t Menu (void *data, uint8_t x, uint8_t y, uint16_t spos, uint8_t len,
               uint8_t len2, uint16_t count, const String *title)
{
  uint16_t result = 0;
  uint16_t len2b;

  DBG_ENTER ("Menu");

  if (count == 0)
    goto _exit;

  _mnu.max = Length (title);
  _mnu.data = data;
  _mnu.count = count;
  _mnu.len = len;

  if (!mn_environment.unpolite)
    ScreenMemCopy (mn_environment.v_dest, ptr_scr_backup2);

  _mnu.vscrollbar_pos = UINT16_NULL;

  if (!mn_environment.preview)
    HideCursor ();

  {
    bool key_used = false;

    for (uint16_t i = 0; i < count; i++)
    {
      String_t s = Menu_pstr (i + 1);

      _mnu.mbuf[i].key = OutKey ((String *)&s);
      _mnu.mbuf[i].use = (_mnu.mbuf[i].key == '~') != !mn_setting.reverse_use;
      if (_mnu.mbuf[i].use)
        key_used = true;
    }

    if (key_used)
      _mnu.solid = false;
    else
    {
      for (uint16_t i = 0; i < count; i++)
        _mnu.mbuf[i].use = true;

      _mnu.solid = true;
    }
  }

  for (uint16_t i = 0; i < count; i++)
  {
    String_t s = Menu_pstr (i + 1);
    uint16_t m = CStrLen ((String *)&s) + mn_environment.descr_len;

    if (_mnu.max < m)
      _mnu.max = m;
  }

  if (mn_setting.center_box)
  {
    x = ((work_MaxCol - _mnu.max - 2) / 2) + ((work_MaxCol - _mnu.max - 2) % 2);
    y = ((work_MaxLn - len2 - 1) / 2) + ((work_MaxLn - len2 - 1) % 2);
  }

  _mnu.x = x;
  _mnu.y = y;
  len2b = len2;
  mn_environment.xpos = x;
  mn_environment.ypos = y;
  mn_environment.xsize = _mnu.max + 1;
  mn_environment.ysize = len2 + 1;
  mn_environment.desc_pos = y + len2 + 1;

  if (!mn_environment.unpolite)
  {
    bool old_fr_shadow_enabled = fr_setting.shadow_enabled;

    fr_setting.shadow_enabled = mn_setting.shadow_enabled;

    if (mn_environment.intact_area)
      fr_setting.update_area = false;
    if (mn_setting.frame_enabled)
      Frame (mn_environment.v_dest, x, y, x + _mnu.max + 1, y + len2 + 1, mn_setting.menu_attr,
             title, mn_setting.title_attr, mn_setting.frame_type);
    if (mn_environment.intact_area)
      fr_setting.update_area = true;

    fr_setting.shadow_enabled = old_fr_shadow_enabled;

    if (mn_setting.frame_enabled)
    {
      String_t contxt, s, t;

      contxt = DietStr (mn_environment.context, _mnu.max + (Length (mn_environment.context) - CStrLen (mn_environment.context)));
      CopyString ((String *)&s, (String *)&"\x01" "`", 255); // color switch
      t = ExpStrR ((String *)"\x00", _mnu.max - CStrLen ((String *)&contxt), GetStr (mn_setting.frame_type)[1]);
      AppendString ((String *)&s, (String *)&t, 255);
      AppendString ((String *)&s, (String *)"\x01" "`", 255); // color switch
      AppendString ((String *)&s, (String *)&contxt, 255);

      ShowC3Str (mn_environment.v_dest, x + 1, y + len2 + 1,
                 (String *)&s,
                 mn_setting.contxt_attr,
                 mn_setting.contxt2_attr,
                 mn_setting.menu_attr);
    }

    _mnu.vscrollbar_size = len2;
    _mnu.len2 = len2;

    if (len2 > count) len2 = count;
    if (len2 < 1)  len2 = 1;
    if (spos < 1) spos = 1;
    if (spos > count) spos = count;

    mn_environment.refresh = Menu_refresh;

    _mnu.first = 1;
    _mnu.last = count;
    while (!_mnu.mbuf[_mnu.first - 1].use) _mnu.first++;
    while (!_mnu.mbuf[_mnu.last - 1].use) _mnu.last--;

    if (_mnu.first <= mn_setting.topic_len)
      _mnu.first = mn_setting.topic_len + 1;
    if ((spos < _mnu.first) || (spos > _mnu.last))
      spos = _mnu.first;
    _mnu.idx2 = 1;
    _mnu.page = 1;
    _mnu.opage = UINT16_NULL;
    _mnu.opos = UINT16_NULL;
    while ((_mnu.idx2 + _mnu.page - 1) < spos)
      Menu_AddPos (&_mnu.idx2, len2);
  }

  _mnu.topic_len = mn_setting.topic_len;
  if (_mnu.topic_len != 0)
  {
    mn_setting.topic_len = 0;

    Menu_refresh ();

    mn_setting.topic_len = _mnu.topic_len;
    _mnu.topic_len = 0;
    _mnu.data = (uint8_t *)_mnu.data + (len + 1) * mn_setting.topic_len;
    _mnu.y += mn_setting.topic_len;
    len2 -= mn_setting.topic_len;
    _mnu.len2 -= mn_setting.topic_len;
    memmove (_mnu.mbuf, &_mnu.mbuf[mn_setting.topic_len], (count - mn_setting.topic_len) * sizeof (_mnu.mbuf[0]));

    for (uint16_t i = 0; i < mn_setting.topic_len; i++)
      Menu_SubPos (&_mnu.idx2);

    count -= mn_setting.topic_len;
    _mnu.count -= mn_setting.topic_len;
    _mnu.first -= mn_setting.topic_len;
    _mnu.last -= mn_setting.topic_len;
  }

  Menu_refresh ();

  mn_environment.curr_page = _mnu.page;
  mn_environment.curr_pos = _mnu.idx2 + _mnu.page - 1;
  {
    String_t s, t;
    t = Menu_pstr (_mnu.idx2 + _mnu.page - 1);
    s = CutStr ((String *)&t);
    CopyString (mn_environment.curr_item, (String *)&s, 255);
  }
  mn_environment.keystroke = ATEK_UNKNOWN;
  if (mn_environment.ext_proc != NULL)
    mn_environment.ext_proc ();

  if (mn_environment.preview)
  {
    mn_environment.preview  = false;
    mn_environment.unpolite = true;
  }
  else
  {
    bool qflg = false;
    ExtKeyCode key = mn_environment.keystroke;

    do
    {
      mn_environment.keystroke = key;
      key = GetKey ();
      if (!qflg)
      {
        if (((key & 0xFF) >= 0x20)/*&& ((key & 0xFF) <= 0xFF)*/)
        {
          _mnu.idx = Menu_RetKey (key & 0xFF, count);
          if (_mnu.idx != 0)
          {
            Menu_refresh ();
            _mnu.idx2 = _mnu.idx;
            if (!((key == mn_setting.terminate_keys[1]) && mn_setting.edit_contents))
              qflg = true;
            else
              Menu_edit_contents (_mnu.idx, x, y, len);
          }
        }
        else if (!shift_pressed () && !ctrl_pressed () && !alt_pressed ())
        {
          switch (key)
          {
            case kUP:
              if ((_mnu.page + _mnu.idx2 - 1 > _mnu.first) || !mn_setting.cycle_moves)
                Menu_SubPos (&_mnu.idx2);
              else
              {
                _mnu.idx2 = len2;
                _mnu.page = count - len2 + 1;
                if (!_mnu.mbuf[_mnu.idx2 + _mnu.page - 1 - 1].use)
                  Menu_SubPos (&_mnu.idx2);
              }
              break;

            case kDOWN:
              if ((_mnu.page + _mnu.idx2 - 1 < _mnu.last) || !mn_setting.cycle_moves)
                Menu_AddPos (&_mnu.idx2, len2);
              else
              {
                _mnu.idx2 = 1;
                _mnu.page = 1;
                if (!_mnu.mbuf[_mnu.idx2 + _mnu.page - 1 - 1].use)
                  Menu_AddPos (&_mnu.idx2, len2);
              }
              break;

            case kHOME:
              if (mn_setting.homing_pos == 0)
              {
                _mnu.idx2 = 1;
                _mnu.page = 1;
              }
              else if ((_mnu.idx2 + _mnu.page - 1 > mn_setting.homing_pos) && (mn_setting.homing_pos < count))
              {
                do
                {
                  Menu_SubPos (&_mnu.idx2);
                } while (_mnu.idx2 + _mnu.page - 1 > mn_setting.homing_pos);
              }
              else
              {
                _mnu.idx2 = 1;
                _mnu.page = 1;
              }
              if (!_mnu.mbuf[_mnu.idx2 + _mnu.page - 1 - 1].use)
                Menu_AddPos (&_mnu.idx2, len2);
              break;

            case kEND:
              if (mn_setting.homing_pos == 0)
              {
                _mnu.idx2 = len2;
                _mnu.page = count - len2 + 1;
              }
              else if ((_mnu.idx2 + _mnu.page - 1 < mn_setting.homing_pos) && (mn_setting.homing_pos < count))
              {
                do
                {
                  Menu_AddPos (&_mnu.idx2, len2);
                } while (_mnu.idx2 + _mnu.page - 1 < mn_setting.homing_pos);
              }
              else
              {
                _mnu.idx2 = len2;
                _mnu.page = count - len2 + 1;
              }
              if (!_mnu.mbuf[_mnu.idx2 + _mnu.page - 1 - 1].use)
                Menu_SubPos (&_mnu.idx2);
              break;

            case kPgUP:
              if ((_mnu.idx2 + _mnu.page - 1 - (len2 - 1) > mn_setting.homing_pos)
                  || (_mnu.idx2 + _mnu.page - 1 <= mn_setting.homing_pos)
                  || (mn_setting.homing_pos == 0) || (mn_setting.homing_pos >= count))
              {
                for (uint16_t i = 1; i < len2; i++)
                  Menu_SubPos (&_mnu.idx2);
              }
              else
              {
                do
                {
                  Menu_SubPos (&_mnu.idx2);
                } while (_mnu.idx2 + _mnu.page - 1 > mn_setting.homing_pos);
              }
              break;

            case kPgDOWN:
              if ((_mnu.idx2 + _mnu.page - 1 + (len2 - 1) < mn_setting.homing_pos)
                  || (_mnu.idx2 + _mnu.page - 1 >= mn_setting.homing_pos)
                  || (mn_setting.homing_pos == 0)
                  || (mn_setting.homing_pos >= count))
              {
                for (uint16_t i = 1; i < len2; i++)
                  Menu_AddPos (&_mnu.idx2, len2);
              }
              else
              {
                do
                {
                  Menu_AddPos (&_mnu.idx2, len2);
                } while (_mnu.idx2 + _mnu.page - 1 < mn_setting.homing_pos);
              }
              break;

            default:
              break;
          }
        }
      }

      if (LookUpKey (key, mn_setting.terminate_keys, 50))
      {
        if (!((key == mn_setting.terminate_keys[1]) && mn_setting.edit_contents))
          qflg = true;
        else
          Menu_edit_contents (_mnu.idx2 + _mnu.page - 1, x, y, len);
      }

      mn_environment.curr_page = _mnu.page;
      mn_environment.curr_pos = _mnu.idx2 + _mnu.page - 1;
      {
        String_t s, t;

        t = Menu_pstr (_mnu.idx2 + _mnu.page - 1);
        s = CutStr ((String *)&t);
        CopyString (mn_environment.curr_item, (String *)&s, 255);
      }
      Menu_refresh ();
      mn_environment.keystroke = key;
      if (mn_environment.ext_proc != NULL)
        mn_environment.ext_proc ();
#if !GO32
      draw_screen ();
#endif // !GO32
    } while (!qflg && !_force_program_quit);
  }

  if (mn_environment.winshade && !mn_environment.unpolite)
  {
    if (move_to_screen_routine != NULL)
    {
        move_to_screen_data = ptr_scr_backup2;
        move_to_screen_area[0] = x;
        move_to_screen_area[1] = y;
        move_to_screen_area[2] = x + _mnu.max + 1 + 2;
        move_to_screen_area[3] = y + len2b + 1 + 1;
        move_to_screen_routine ();
    }
    else
      ScreenMemCopy (ptr_scr_backup2, mn_environment.v_dest);
  }

  result = _mnu.idx2 + _mnu.page - 1;

_exit:
  DBG_LEAVE (); //EXIT //Menu
  return result;
}
