// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void Menu_edit_contents (uint16_t item, uint8_t x, uint8_t y, uint8_t len)
{
  String_t item_str, t;

  const CharSet_t _charset =
  { // ' '..'_', 'a'..'}', 128..255
    0x00, 0x00,
    0x00, 0x00,
    0xFF, 0xFF, // ' '..'_'
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF,
    MK_UINT8 (0,1,1,1,1,1,1,1), MK_UINT8 (1,1,1,1,1,1,1,1), // 'a'..'o' (excluding '`')
    MK_UINT8 (1,1,1,1,1,1,1,1), MK_UINT8 (1,1,1,1,1,1,0,0), // 'p'..'}' (excluding '~' and 127)
    0xFF, 0xFF, // 128..255
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF
  };

  DBG_ENTER ("Menu_edit_contents");

  is_setting.append_enabled = true;
  memcpy (is_setting.character_set, _charset, sizeof (CharSet_t));
  is_environment.locate_pos = 1;

  item_str = Menu_pstr (item);

  if ((mn_environment.edit_pos > 0) && (mn_environment.edit_pos < (_mnu.max - 2)))
    t = Copy ((String *)&item_str,
              mn_environment.edit_pos + 1,
              item_str.len - mn_environment.edit_pos + 1);
  else
    t = CutStr ((String *)&item_str);

  mn_environment.is_editing = true;

  while ((t.len != 0) && (t.str[t.len - 1] == ' '))
    Delete ((String *)&t, t.len, 1);

  t = InputStr ((String *)&t, x + 1 + mn_environment.edit_pos, y + _mnu.idx2,
                _mnu.max - 2 - mn_environment.edit_pos + 1,
                _mnu.max - 2 - mn_environment.edit_pos + 1,
                mn_setting.text2_attr, mn_setting.default_attr);
  mn_environment.is_editing = false;

  HideCursor ();
  if (is_environment.keystroke == kENTER)
  {
    if (   (mn_environment.edit_pos > 0)
        && (mn_environment.edit_pos < (_mnu.max - 2)))
    {
      String_t s = Copy ((String *)&item_str, 1, mn_environment.edit_pos);

      AppendString ((String *)&s, (String *)&t, 255);
      CopyString ((String *)&t, (String *)&s, 255);
    }
    else
      t = CutStr ((String *)&t);

    memmove ((uint8_t *)_mnu.data + (item - 1) * (len + 1), &t, len + 1);
  }

  mn_environment.do_refresh = true;
  Menu_refresh ();

  DBG_LEAVE (); //EXIT //Menu_edit_contents
}
