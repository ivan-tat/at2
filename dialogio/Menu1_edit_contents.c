// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void Menu1_edit_contents (uint16_t item, uint8_t x, uint8_t y, uint8_t len)
{
  String_t item_str, s;

  const CharSet_t _charset =
  { // 32..255
    0x00, 0x00,
    0x00, 0x00,
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF
  };

  DBG_ENTER ("Menu1_edit_contents");

  is_setting.append_enabled = true;
  memcpy (is_setting.character_set, _charset, sizeof (CharSet_t));
  is_environment.locate_pos = 1;

  item_str = Menu1_pstr (item);

  if ((mn1_environment.edit_pos > 0) && (mn1_environment.edit_pos < _mnu1.max - 2))
    s = Copy ((String *)&item_str,
              mn1_environment.edit_pos + 1,
              item_str.len - mn1_environment.edit_pos + 1);
  else
    s = CutStr ((String *)&item_str);

  remove_spaces_right ((String *)&s);

  s = InputStr ((String *)&s,
                x + 1 + mn1_environment.edit_pos,
                y + _mnu1.idx2,
                _mnu1.max - 2 - mn1_environment.edit_pos + 1,
                _mnu1.max - 2 - mn1_environment.edit_pos + 1,
                mn1_setting.text2_attr,
                mn1_setting.default_attr);

  if (is_environment.keystroke == kENTER)
  {
    if (   (mn1_environment.edit_pos > 0)
        && (mn1_environment.edit_pos < _mnu1.max - 2))
    {
      String_t t;

      //item_str = Menu1_pstr (item);
      t = Copy ((String *)&item_str, 1, mn1_environment.edit_pos);
      AppendString ((String *)&t, (String *)&s, 255);
      CopyString ((String *)&s, (String *)&t, 255);
    }
    else
      s = CutStr ((String *)&s);

    memmove ((uint8_t *)_mnu1.data + (item - 1) * (len + 1), &s, len + 1);
  }

  //item_str = Menu1_pstr (item);
  s = ExpStrR ((String *)&item_str, _mnu1.max + item_str.len - CStrLen ((String *)&item_str), ' ');
  ShowCStr (mn1_environment.v_dest,
            x + 1,
            y + _mnu1.idx2,
            (String *)&s,
            mn1_setting.text2_attr,
            mn1_setting.short2_attr);

  DBG_LEAVE (); //EXIT //Menu1_edit_contents
}
