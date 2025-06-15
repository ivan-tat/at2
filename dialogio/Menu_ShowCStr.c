// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void Menu_ShowCStr (tSCREEN_MEM *dest, uint8_t x, uint8_t y,
                           const String *str,
                           uint8_t attr1, uint8_t attr2,
                           uint8_t attr3, uint8_t attr4)
{
  DBG_ENTER ("Menu_ShowCStr");

  if (mn_setting.fixed_len == 0)
  {
    ShowC3Str (dest, x, y, str, attr1, attr2,
               (attr1 & 0xF0) + (mn_setting.topic_attr & 0x0F));
  }
  else
  {
    bool highlighted = false;
    size_t sl = Length (str);
    size_t len = 0, len2 = 0;
    String s[1+1];

    SetLength (s, 1);

    for (size_t i = 1; i <= sl; i++)
      if (GetStr (str)[i - 1] == '~')
        highlighted = !highlighted;
      else
      {
        GetStr (s)[0] = GetStr (str)[i - 1];

        if (   (i >= mn_environment.fixed_start)
            && (len < mn_setting.fixed_len))
        {
          ShowStr (dest, x + len2, y, s, !highlighted ? attr1 : attr2);
          len++;
        }
        else
        {
          ShowStr (dest, x + len2, y, s, !highlighted ? attr3 : attr4);
          if (i >= mn_environment.fixed_start)
            len++;
        }
        len2++;
      }
  }

  DBG_LEAVE (); //EXIT //Menu_ShowCStr
}
