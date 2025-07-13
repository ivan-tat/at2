// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

uint16_t Menu2 (void *data, uint8_t x, uint8_t y, uint16_t spos, uint8_t len,
                uint8_t len2, uint16_t count, const String *title)
{
  uint16_t result = 0;
  uint16_t len2b;

  DBG_ENTER ("Menu2");

  if (count == 0)
    goto _exit;

  _mnu2.max = Length (title);
  _mnu2.data = data;
  _mnu2.count = count;
  _mnu2.len = len;

  if (!mn2_environment.unpolite)
    ScreenMemCopy (mn2_environment.v_dest, ptr_scr_backup);

  _mnu2.vscrollbar_pos = BYTE_NULL;

  if (!mn2_environment.preview)
    HideCursor ();

  {
    bool key_used = false;

    for (uint16_t i = 0; i < count; i++)
    {
      String_t t = Menu2_pstr (i + 1);

      _mnu2.mbuf[i].key = OutKey ((String *)&t);
      _mnu2.mbuf[i].use = (_mnu2.mbuf[i].key == '~') == mn2_setting.reverse_use;

      if (_mnu2.mbuf[i].use)
        key_used = true;
    }

    if (key_used)
      _mnu2.solid = false;
    else
    {
      for (uint16_t i = 0; i < count; i++)
        _mnu2.mbuf[i].use = true;

      _mnu2.solid = true;
    }
  }

  for (uint16_t i = 0; i < count; i++)
  {
    String_t t = Menu2_pstr (i + 1);
    size_t max = CStrLen ((String *)&t) + mn2_environment.descr_len;

    if (_mnu2.max < max)
      _mnu2.max = max;
  }

  if (mn2_setting.center_box)
  {
    x = ((work_MaxCol - _mnu2.max - 2) / 2) + ((work_MaxCol - _mnu2.max - 2) % 2);
    y = ((work_MaxLn - len2 - 1) / 2) + ((work_MaxLn - len2 - 1) % 2);
  }

  _mnu2.x = x;
  _mnu2.y = y;
  len2b = len2;

  if (!mn2_environment.unpolite)
  {
    bool old_fr_shadow_enabled = fr_setting.shadow_enabled;
    String_t contxt;

    fr_setting.shadow_enabled = mn2_setting.shadow_enabled;
    fr_setting.update_area = false;
    Frame (mn2_environment.v_dest,
           x,
           y,
           x + _mnu2.max + 1,
           y + len2 + 1,
           mn2_setting.menu_attr,
           title,
           mn2_setting.title_attr,
           mn2_setting.frame_type);
    fr_setting.update_area = true;
    fr_setting.shadow_enabled = old_fr_shadow_enabled;

    contxt = DietStr (mn2_environment.context,
                      _mnu2.max
                      + Length (mn2_environment.context)
                      - CStrLen (mn2_environment.context));
    ShowCStr (mn2_environment.v_dest,
              x + _mnu2.max + 1 - CStrLen ((String *)&contxt),
              y + len2 + 1,
              (String *)&contxt,
              mn2_setting.contxt_attr,
              mn2_setting.contxt2_attr);

    _mnu2.vscrollbar_size = len2;
    if (len2 > count) len2 = count;
    if (len2 < 1) len2 = 1;
    if (spos < 1) spos = 1;
    if (spos > count) spos = count;

    _mnu2.len2 = len2;
    mn2_environment.refresh = Menu2_refresh;

    _mnu2.first = 1;
    _mnu2.last = count;
    while (!_mnu2.mbuf[_mnu2.first - 1].use) _mnu2.first++;
    while (!_mnu2.mbuf[_mnu2.last - 1].use) _mnu2.last--;

    if ((spos < _mnu2.first) || (spos > _mnu2.last))
      spos = _mnu2.first;
    _mnu2.idx2 = 1;
    _mnu2.page = 1;
    _mnu2.opage = UINT16_NULL;
    _mnu2.opos = UINT16_NULL;
    while (_mnu2.idx2 + _mnu2.page - 1 < spos)
      Menu2_AddPos (&_mnu2.idx2, len2);
  }

  mn2_environment.curr_pos = _mnu2.idx2 + _mnu2.page - 1;
  mn2_environment.keystroke = ATEK_UNKNOWN;
  if (mn2_environment.ext_proc != NULL)
    mn2_environment.ext_proc ();

  if (mn2_environment.preview)
  {
    mn2_environment.preview  = false;
    mn2_environment.unpolite = true;
    Menu2_refresh ();
  }
  else
  {
    bool qflg = false;

    do
    {
      ExtKeyCode key = GetKey ();

      if (LookUpKey (key, mn2_setting.terminate_keys, 50))
      {
        if (!(   (key == mn2_setting.terminate_keys[1])
              && mn2_setting.edit_contents))
          qflg = true;
        else
          Menu2_edit_contents (_mnu2.idx2 + _mnu2.page - 1, x, y, len);
      }

      if (!qflg)
      {
        if (((key & 0xFF) >= 0x20)/* && ((key & 0xFF) <= 0xFF)*/)
        {
          _mnu2.idx = Menu2_RetKey (key & 0xFF, count);
          if (_mnu2.idx != 0)
          {
            Menu2_refresh ();
            _mnu2.idx2 = _mnu2.idx;
            if (!(   (key == mn2_setting.terminate_keys[1])
                  && mn2_setting.edit_contents))
              qflg = true;
            else
              Menu2_edit_contents (_mnu2.idx, x, y, len);
          }
        }
        else if (!shift_pressed () && !ctrl_pressed () && !alt_pressed ())
          switch (key)
          {
            case kUP:
              if ((_mnu2.page + _mnu2.idx2 - 1 > _mnu2.first) || !mn2_setting.cycle_moves)
                Menu2_SubPos (&_mnu2.idx2);
              else
              {
                _mnu2.idx2 = len2;
                _mnu2.page = count - len2 + 1;
                if (!_mnu2.mbuf[_mnu2.idx2 + _mnu2.page - 1 - 1].use)
                  Menu2_SubPos (&_mnu2.idx2);
              }
              break;

            case kDOWN:
              if (   (_mnu2.page + _mnu2.idx2 - 1 < _mnu2.last)
                  || !mn2_setting.cycle_moves)
                Menu2_AddPos (&_mnu2.idx2, len2);
              else
              {
                _mnu2.idx2 = 1;
                _mnu2.page = 1;
                if (!_mnu2.mbuf[_mnu2.idx2 + _mnu2.page - 1 - 1].use)
                  Menu2_AddPos (&_mnu2.idx2, len2);
              }
              break;

            case kHOME:
              _mnu2.idx2 = 1;
              _mnu2.page = 1;
              if (!_mnu2.mbuf[_mnu2.idx2 + _mnu2.page - 1 - 1].use)
                Menu2_AddPos (&_mnu2.idx2, len2);
              break;

            case kEND:
              _mnu2.idx2 = len2;
              _mnu2.page = count - len2 + 1;
              if (!_mnu2.mbuf[_mnu2.idx2 + _mnu2.page - 1 - 1].use)
                Menu2_SubPos (&_mnu2.idx2);
              break;

            case kPgUP:
              for (uint16_t i = 1; i < len2; i++)
                Menu2_SubPos (&_mnu2.idx2);
              break;

            case kPgDOWN:
              for (uint16_t i = 1; i < len2; i++)
                Menu2_AddPos (&_mnu2.idx2, len2);
              break;

            default:
              break;
          }
      }

      mn2_environment.curr_pos = _mnu2.idx2 + _mnu2.page - 1;
      Menu2_refresh ();
      mn2_environment.keystroke = key;
      if (mn2_environment.ext_proc != NULL)
        mn2_environment.ext_proc ();
#if GO32
      //draw_screen ();
      keyboard_reset_buffer_alt ();
#else // !GO32
      draw_screen ();
      //keyboard_reset_buffer ();
#endif // !GO32
    } while (!qflg && !_force_program_quit);
  }

  result = _mnu2.idx2 + _mnu2.page - 1;

  if (mn2_environment.winshade && !mn2_environment.unpolite)
  {
    if (move_to_screen_routine != NULL)
    {
      move_to_screen_data = ptr_scr_backup;
      move_to_screen_area[0] = x;
      move_to_screen_area[1] = y;
      move_to_screen_area[2] = x + _mnu2.max + 1 + 2;
      move_to_screen_area[3] = y + len2b + 1 + 1;
      move_to_screen_routine ();
    }
    else
      ScreenMemCopy (ptr_scr_backup, mn2_environment.v_dest);
  }

_exit:
  DBG_LEAVE (); //EXIT //Menu2
  return result;
}
