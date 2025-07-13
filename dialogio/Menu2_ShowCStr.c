// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void Menu2_ShowCStr (tSCREEN_MEM *dest, uint8_t x, uint8_t y,
                            const String *str,
                            uint8_t atr1, uint8_t atr2,
                            uint8_t atr3, uint8_t atr4)
{
  DBG_ENTER ("Menu2_ShowCStr");

  if (mn2_setting.fixed_len == 0)
    ShowCStr (dest, x, y, str, atr1, atr2);
  else
  {
    bool highlighted = false;
    size_t sl = Length (str);
    size_t len = 0;
    String s[1+1];

    SetLength (s, 1);

    for (size_t i = 0; i < sl; i++)
      if (GetStr (str)[i] == '~')
        highlighted = !highlighted;
      else
      {
        uint8_t attr;

        GetStr (s)[0] = GetStr (str)[i];

        if (len < mn2_setting.fixed_len)
          attr = highlighted ? atr2 : atr1;
        else
          attr = highlighted ? atr4 : atr3;

        ShowStr (dest, x, y, s, attr);
        len++;
        x++;
      }
  }

  DBG_LEAVE (); //EXIT //Menu2_ShowCStr
}
