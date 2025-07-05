// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

uint16_t Menu1 (void *data, uint8_t x, uint8_t y, uint16_t spos, uint8_t len,
                uint8_t len2, uint16_t count, const String *title)
{
  uint16_t result = 0;
  uint16_t len2b;

  DBG_ENTER ("Menu1");

  if (count == 0)
    goto _exit;

  _mnu1.max = Length (title);
  _mnu1.data = data;
  _mnu1.count = count;
  _mnu1.len = len;

  if (!mn1_environment.unpolite)
    ScreenMemCopy (mn1_environment.v_dest, ptr_scr_backup);

  _mnu1.vscrollbar_pos = BYTE_NULL;

  if (!mn1_environment.preview)
    HideCursor ();

  {
    bool key_used = false;

    for (uint16_t i = 0; i < count; i++)
    {
      String_t t = Menu1_pstr (i + 1);

      _mnu1.mbuf[i].key = OutKey ((String *)&t);
      _mnu1.mbuf[i].use = (_mnu1.mbuf[i].key == '~') == mn1_setting.reverse_use;

      if (_mnu1.mbuf[i].use)
        key_used = true;
    }

    if (key_used)
      _mnu1.solid = false;
    else
    {
      for (uint16_t i = 0; i < count; i++)
        _mnu1.mbuf[i].use = true;

      _mnu1.solid = true;
    }
  }

  for (uint16_t i = 0; i < count; i++)
  {
    String_t t = Menu1_pstr (i + 1);
    size_t max = CStrLen ((String *)&t) + mn1_environment.descr_len;

    if (_mnu1.max < max)
      _mnu1.max = max;
  }

  if (mn1_setting.center_box)
  {
    x = ((work_MaxCol - _mnu1.max - 2) / 2) + ((work_MaxCol - _mnu1.max - 2) % 2);
    y = ((work_MaxLn - len2 - 1) / 2) + ((work_MaxLn - len2 - 1) % 2);
  }

  _mnu1.x = x;
  _mnu1.y = y;
  len2b = len2;

  if (!mn1_environment.unpolite)
  {
    bool old_fr_shadow_enabled = fr_setting.shadow_enabled;
    String_t contxt;

    fr_setting.shadow_enabled = mn1_setting.shadow_enabled;
    fr_setting.update_area = false;
    Frame (mn1_environment.v_dest,
           x,
           y,
           x + _mnu1.max + 1,
           y + len2 + 1,
           mn1_setting.menu_attr,
           title,
           mn1_setting.title_attr,
           mn1_setting.frame_type);
    fr_setting.update_area = true;
    fr_setting.shadow_enabled = old_fr_shadow_enabled;

    contxt = DietStr (mn1_environment.context,
                      _mnu1.max
                      + Length (mn1_environment.context)
                      - CStrLen (mn1_environment.context));
    ShowCStr (mn1_environment.v_dest,
              x + _mnu1.max + 1 - CStrLen ((String *)&contxt),
              y + len2 + 1,
              (String *)&contxt,
              mn1_setting.contxt_attr,
              mn1_setting.contxt2_attr);

    _mnu1.vscrollbar_size = len2;
    if (len2 > count) len2 = count;
    if (len2 < 1) len2 = 1;
    if (spos < 1) spos = 1;
    if (spos > count) spos = count;

    _mnu1.len2 = len2;
    mn1_environment.refresh = Menu1_refresh;

    _mnu1.first = 1;
    _mnu1.last = count;
    while (!_mnu1.mbuf[_mnu1.first - 1].use) _mnu1.first++;
    while (!_mnu1.mbuf[_mnu1.last - 1].use) _mnu1.last--;

    if ((spos < _mnu1.first) || (spos > _mnu1.last))
      spos = _mnu1.first;
    _mnu1.idx2 = 1;
    _mnu1.page = 1;
    _mnu1.opage = UINT16_NULL;
    _mnu1.opos = UINT16_NULL;
    while (_mnu1.idx2 + _mnu1.page - 1 < spos)
      Menu1_AddPos (&_mnu1.idx2, len2);
  }

  mn1_environment.curr_pos = _mnu1.idx2 + _mnu1.page - 1;
  mn1_environment.keystroke = ATEK_UNKNOWN;
  if (mn1_environment.ext_proc != NULL)
    mn1_environment.ext_proc ();

  if (mn1_environment.preview)
  {
    mn1_environment.preview  = false;
    mn1_environment.unpolite = true;
    Menu1_refresh ();
  }
  else
  {
    bool qflg = false;

    do
    {
      ExtKeyCode key = GetKey ();

      if (LookUpKey (key, mn1_setting.terminate_keys, 50))
      {
        if (!(   (key == mn1_setting.terminate_keys[1])
              && mn1_setting.edit_contents))
          qflg = true;
        else
          Menu1_edit_contents (_mnu1.idx2 + _mnu1.page - 1, x, y, len);
      }

      if (!qflg)
      {
        if (((key & 0xFF) >= 0x20)/* && ((key & 0xFF) <= 0xFF)*/)
        {
          _mnu1.idx = Menu1_RetKey (key & 0xFF, count);
          if (_mnu1.idx != 0)
          {
            Menu1_refresh ();
            _mnu1.idx2 = _mnu1.idx;
            if (!(   (key == mn1_setting.terminate_keys[1])
                  && mn1_setting.edit_contents))
              qflg = true;
            else
              Menu1_edit_contents (_mnu1.idx, x, y, len);
          }
        }
        else if (!shift_pressed () && !ctrl_pressed () && !alt_pressed ())
          switch (key)
          {
            case kUP:
              if ((_mnu1.page + _mnu1.idx2 - 1 > _mnu1.first) || !mn1_setting.cycle_moves)
                Menu1_SubPos (&_mnu1.idx2);
              else
              {
                _mnu1.idx2 = len2;
                _mnu1.page = count - len2 + 1;
                if (!_mnu1.mbuf[_mnu1.idx2 + _mnu1.page - 1 - 1].use)
                  Menu1_SubPos (&_mnu1.idx2);
              }
              break;

            case kDOWN:
              if (   (_mnu1.page + _mnu1.idx2 - 1 < _mnu1.last)
                  || !mn1_setting.cycle_moves)
                Menu1_AddPos (&_mnu1.idx2, len2);
              else
              {
                _mnu1.idx2 = 1;
                _mnu1.page = 1;
                if (!_mnu1.mbuf[_mnu1.idx2 + _mnu1.page - 1 - 1].use)
                  Menu1_AddPos (&_mnu1.idx2, len2);
              }
              break;

            case kHOME:
              _mnu1.idx2 = 1;
              _mnu1.page = 1;
              if (!_mnu1.mbuf[_mnu1.idx2 + _mnu1.page - 1 - 1].use)
                Menu1_AddPos (&_mnu1.idx2, len2);
              break;

            case kEND:
              _mnu1.idx2 = len2;
              _mnu1.page = count - len2 + 1;
              if (!_mnu1.mbuf[_mnu1.idx2 + _mnu1.page - 1 - 1].use)
                Menu1_SubPos (&_mnu1.idx2);
              break;

            case kPgUP:
              for (uint16_t i = 1; i < len2; i++)
                Menu1_SubPos (&_mnu1.idx2);
              break;

            case kPgDOWN:
              for (uint16_t i = 1; i < len2; i++)
                Menu1_AddPos (&_mnu1.idx2, len2);
              break;

            default:
              break;
          }
      }

      mn1_environment.curr_pos = _mnu1.idx2 + _mnu1.page - 1;
      Menu1_refresh ();
      mn1_environment.keystroke = key;
      if (mn1_environment.ext_proc != NULL)
        mn1_environment.ext_proc ();
#if GO32
      //draw_screen ();
      keyboard_reset_buffer_alt ();
#else // !GO32
      draw_screen ();
      //keyboard_reset_buffer ();
#endif // !GO32
    } while (!qflg && !_force_program_quit);
  }

  result = _mnu1.idx2 + _mnu1.page - 1;

  if (mn1_environment.winshade && !mn1_environment.unpolite)
  {
    if (move_to_screen_routine != NULL)
    {
      move_to_screen_data = ptr_scr_backup;
      move_to_screen_area[0] = x;
      move_to_screen_area[1] = y;
      move_to_screen_area[2] = x + _mnu1.max + 1 + 2;
      move_to_screen_area[3] = y + len2b + 1 + 1;
      move_to_screen_routine ();
    }
    else
      ScreenMemCopy (ptr_scr_backup, mn1_environment.v_dest);
  }

_exit:
  DBG_LEAVE (); //EXIT //Menu1
  return result;
}
