// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

uint8_t Dialog (const String *text, const String *keys, const String *title, uint8_t spos)
{
  uint8_t result = 0;
  bool is_string_input;
  uint16_t pos = 1;
  uint16_t max;
  uint16_t mx2;
  // used when `is_string_input' = true (initialized to prevent compiler's warnings):
  uint16_t ln = 0;
  uint16_t ln1 = 0;
  uint16_t atr1 = 0;
  uint16_t atr2 = 0;

  DBG_ENTER ("Dialog");

  max = Length (title);
  _dlg.num = 0;

  ScreenMemCopy (screen_ptr, ptr_scr_backup);
  HideCursor ();

  {
    size_t e = Length (text);
    size_t l; // length of `_dlg.str'

    do
    {
      size_t cl; // length of `_dlg.str' without color marks

      ReadChunk ((String *)&_dlg.str, text, pos);
      l = _dlg.str.len;
      pos += l + 1;
      cl = CStrLen ((String *)&_dlg.str);
      if (cl > max)
        max = cl;
      if (l != 0)
        _dlg.num++;
    } while ((pos < e) && (l != 0));
  }

  pos = 1;
  mx2 = 0;
  _dlg.nm2 = 0;

  {
    String_t t = Copy (keys, 1, 14);
    is_string_input = CompareStrings ((String *)&t, (String *)"\x0E" "%string_input%") == 0;
  }

  if (is_string_input)
  {
    pos += 14;

    ReadChunk ((String *)&_dlg.str, keys, pos);
    ln = Str2num ((String *)&_dlg.str, 10);
    if (_dlg.str.len == 0) goto _exit;
    pos += _dlg.str.len + 1;

    ReadChunk ((String *)&_dlg.str, keys, pos);
    ln1 = Str2num ((String *)&_dlg.str, 10);
    mx2 = ln1;
    if (_dlg.str.len == 0) goto _exit;
    pos += _dlg.str.len + 1;

    ReadChunk ((String *)&_dlg.str, keys, pos);
    atr1 = Str2num ((String *)&_dlg.str, 16);
    if (_dlg.str.len == 0) goto _exit;
    pos += _dlg.str.len + 1;

    ReadChunk ((String *)&_dlg.str, keys, pos);
    atr2 = Str2num ((String *)&_dlg.str, 16);
    if (_dlg.str.len == 0) goto _exit;
    pos += _dlg.str.len + 1;
  }
  else
  {
    size_t e = Length (keys);
    size_t l; // length of `_dlg.str'

    do
    {
      ReadChunk ((String *)&_dlg.str, keys, pos);
      l = _dlg.str.len;
      pos += l + 1;
      if (l != 0)
      {
        _dlg.nm2++;
        CopyString ((String *)&_dlg.dbuf[_dlg.nm2 - 1].str, (String *)"\x01" " ", 255);
        AppendString ((String *)&_dlg.dbuf[_dlg.nm2 - 1].str, (String *)&_dlg.str, 255);
        AppendString ((String *)&_dlg.dbuf[_dlg.nm2 - 1].str, (String *)"\x01" " ", 255);
        _dlg.dbuf[_dlg.nm2 - 1].key = OutKey ((String *)&_dlg.str);
        _dlg.dbuf[_dlg.nm2 - 1].use = !dl_setting.all_enabled ? _dlg.dbuf[_dlg.nm2 - 1].key != '~' : true;

        if (_dlg.nm2 > 1)
        {
          _dlg.dbuf[_dlg.nm2 - 1].pos = _dlg.dbuf[_dlg.nm2 - 1 - 1].pos + CStrLen ((String *)&_dlg.dbuf[_dlg.nm2 - 1 - 1].str) + 1;
          mx2 += CStrLen ((String *)&_dlg.dbuf[_dlg.nm2 - 1].str) + 1;
        }
        else
        {
          _dlg.dbuf[_dlg.nm2 - 1].pos = 1;
          mx2 += CStrLen ((String *)&_dlg.dbuf[_dlg.nm2 - 1].str);
        }
      }
    }
    while ((pos < e) && (l != 0));
  }

  if (max < mx2) max = mx2;
  else
  {
    ln1 = max;
    if (ln < max) ln = max;
  }

  if (dl_setting.center_box)
  {
    _dlg.xstart = ((work_MaxCol - (max + 4)) / 2) + ((work_MaxCol - (max + 4)) % 2);
    _dlg.ystart = ((work_MaxLn  - (_dlg.num + 2)) / 2) + ((work_MaxLn  - (_dlg.num + 2)) % 2);
  }
  else
  {
    _dlg.xstart = dl_setting.xstart;
    _dlg.ystart = dl_setting.ystart;
  }

  {
    bool old_fr_shadow_enabled = fr_setting.shadow_enabled;

    fr_setting.shadow_enabled = dl_setting.shadow_enabled;
    Frame (screen_ptr,
           _dlg.xstart, _dlg.ystart, _dlg.xstart + max + 3, _dlg.ystart + _dlg.num + 2,
           dl_setting.box_attr, title, dl_setting.title_attr,
           dl_setting.frame_type);

    fr_setting.shadow_enabled = old_fr_shadow_enabled;
  }

  dl_environment.xpos  = _dlg.xstart;
  dl_environment.ypos  = _dlg.ystart;
  dl_environment.xsize = max + 3;
  dl_environment.ysize = _dlg.num + 2;

  {
    String_t contxt = DietStr (dl_environment.context, max + (Length (dl_environment.context) - CStrLen (dl_environment.context)));

    ShowCStr (screen_ptr, _dlg.xstart + max + 3 - CStrLen ((String *)&contxt), _dlg.ystart + _dlg.num + 2,
              (String *)&contxt, dl_setting.contxt_attr, dl_setting.contxt2_attr);
  }

  pos = 1;
  for (uint_least16_t i = 1; i <= _dlg.num; i++)
  {
    String_t s;
    const String *p;

    ReadChunk ((String *)&_dlg.str, text, pos);
    pos += _dlg.str.len + 1;
    if (dl_setting.center_text)
    {
      s = ExpStrL ((String *)&_dlg.str, _dlg.str.len + (max - CStrLen ((String *)&_dlg.str)) / 2, ' ');
      p = (String *)&s;
    }
    else
      p = (String *)&_dlg.str;

    ShowCStr (screen_ptr, _dlg.xstart + 2, _dlg.ystart + i, p, dl_setting.text_attr, dl_setting.text2_attr);
  }

  if (is_string_input)
  {
    if (use_large_cursor)
      WideCursor ();
    else
      ThinCursor ();
    _dlg.str = InputStr (dl_environment.input_str, _dlg.xstart + 2, _dlg.ystart + _dlg.num + 1, ln, ln1, atr1, atr2);
    if (is_environment.keystroke == kENTER)
      CopyString (dl_environment.input_str, (String *)&_dlg.str, 255);
    dl_environment.keystroke = is_environment.keystroke;
    HideCursor ();
  }
  else
  {
    bool qflg;
    ExtKeyCode key;

    for (uint_least16_t i = 0; i < _dlg.nm2; i++)
    {
      uint8_t attrs[2];
      _dlg.dbuf[i].pos += _dlg.xstart + ((max - mx2) / 2) + 1;
      if (_dlg.dbuf[i].use)
      {
        attrs[0] = dl_setting.keys_attr;
        attrs[1] = dl_setting.short_attr;
      }
      else
      {
        attrs[0] = dl_setting.disbld_attr;
        attrs[1] = dl_setting.disbld_attr;
      }
      ShowCStr (screen_ptr, _dlg.dbuf[i].pos, _dlg.ystart + _dlg.num + 1,
                (String *)&_dlg.dbuf[i].str,
                attrs[0], attrs[1]);
    }

    if (spos < 1)
      spos = 1;
    if (spos > _dlg.nm2)
      spos = _dlg.nm2;

    _dlg.idx2 = spos;
    _dlg.idx3 = 1;

    if (!_dlg.dbuf[_dlg.idx2 - 1].use)
    {
      Dialog_SubPos (&_dlg.idx2);
      if (!_dlg.dbuf[_dlg.idx2 - 1].use)
        Dialog_AddPos (&_dlg.idx2);
    }

    Dialog_ShowItem ();
    Dialog_ShowItem ();

    if (CompareStrings (keys, (String *)"\x01" "$") == 0) goto _exit;

    qflg = false;
    do
    {
      dl_environment.cur_item = _dlg.idx2;
      if (dl_environment.ext_proc != NULL)
        dl_environment.ext_proc ();
      key = GetKey ();
      if (LookUpKey (key, dl_setting.terminate_keys, 50))
        qflg = true;

      if (!qflg)
      {
        if (((key & 0xFF) >= 0x20)/* && ((key & 0xFF) <= 0xFF)*/)
        {
          uint16_t idx = Dialog_RetKey (key & 0xFF);

          if (idx != 0)
          {
            qflg = true;
            _dlg.idx2 = idx;
          }
        }
      }
      else if (!(shift_pressed () || ctrl_pressed () || alt_pressed ()))
        switch (key)
        {
          case kLEFT:
            if ((_dlg.idx2 > 1) || !dl_setting.cycle_moves)
              Dialog_SubPos (&_dlg.idx2);
            else
            {
              _dlg.idx2 = _dlg.nm2;
              if (!_dlg.dbuf[_dlg.idx2 - 1].use)
                Dialog_SubPos (&_dlg.idx2);
            }
            break;

          case kRIGHT:
            if ((_dlg.idx2 < _dlg.nm2) || !dl_setting.cycle_moves)
            {
              uint16_t temp = _dlg.idx2;

              Dialog_AddPos (&_dlg.idx2);
              if (_dlg.idx2 == temp)
              {
                _dlg.idx2 = 1;
                if (!_dlg.dbuf[_dlg.idx2 - 1].use)
                  Dialog_AddPos (&_dlg.idx2);
              }
            }
            else
            {
              _dlg.idx2 = 1;
              if (!_dlg.dbuf[_dlg.idx2 - 1].use)
                Dialog_AddPos (&_dlg.idx2);
            }
            break;

          case kHOME:
            _dlg.idx2 = 1;
            if (!_dlg.dbuf[_dlg.idx2 - 1].use)
              Dialog_AddPos (&_dlg.idx2);
            break;

          case kEND:
            _dlg.idx2 = _dlg.nm2;
            if (!_dlg.dbuf[_dlg.idx2 - 1].use)
              Dialog_SubPos (&_dlg.idx2);
            break;

          default:
            break;
        }

      Dialog_ShowItem ();
#if !GO32
      draw_screen ();
#endif // !GO32
    } while (!qflg && !_force_program_quit);

    result = _dlg.idx2;
    dl_environment.keystroke = key;
  }

  if (move_to_screen_routine != NULL)
  {
    move_to_screen_data = ptr_scr_backup;
    move_to_screen_area[0] = _dlg.xstart;
    move_to_screen_area[1] = _dlg.ystart;
    move_to_screen_area[2] = _dlg.xstart + max + 3 + 2;
    move_to_screen_area[3] = _dlg.ystart + _dlg.num + 2 + 1;
    move_to_screen_routine ();
  }
  else
    ScreenMemCopy (ptr_scr_backup, screen_ptr);

_exit:
  DBG_LEAVE ();
  return result;
}
