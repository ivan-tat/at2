// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

uint16_t VScrollBar (tSCREEN_MEM *dest, uint8_t x, uint8_t y, uint8_t size,
                     uint16_t len1, uint16_t len2, uint16_t pos,
                     uint8_t attr1, uint8_t attr2)
{
  uint16_t temp;

  DBG_ENTER ("VScrollBar");

  if (size > work_MaxLn - y)
    size = work_MaxLn - y;
  if (size < 5)
    size = 5;

  if (size - 2 - 1 < 10)
    temp = _partial (len1, len2, size - 2 - 1);
  else
    temp = _partial (len1, len2, size - 2 - 1 - 2);

  if (pos == temp)
  {
    DBG_LEAVE (); //VScrollBar
    return temp;
  }

  if (size < len1)
  {
    String_t s;
    String t[1+1];

    pos = temp;

    SetLength (t, 1);
    GetStr (t)[0] = charmap.black_up_triangle; // '▲'
    s = ExpStrR (t, size - 2 + 1, charmap.light_shade); // '░'
    //SetLength (t, 1);
    GetStr (t)[0] = charmap.black_down_triangle; // '▼'
    AppendString ((String *)&s, t, size);
    ShowVStr (dest, x, y, (String *)&s, attr1);

    if (size - 2 - 1 < 10)
    {
      s.len = 1;
      s.str[0] = charmap.dark_shade; // '▓'
      ShowStr (dest, x, y + 1 + temp, (String *)&s, attr2);
    }
    else
    {
      s.len = 3;
      s.str[0] = charmap.dark_shade; // '▓'
      s.str[1] = charmap.dark_shade; // '▓'
      s.str[2] = charmap.dark_shade; // '▓'
      ShowVStr (dest, x, y + 1 + temp, (String *)&s, attr2);
    }
  }
  else
  {
    String_t s;
    String t[2+1];

    SetLength (t, 2);
    GetStr (t)[0] = charmap.black_up_triangle; // '▲'
    GetStr (t)[1] = '~'; // switch color
    s = ExpStrR (t, size - 2 + 2, charmap.medium_shade); // '▒'
    //SetLength (t, 2);
    GetStr (t)[0] = '~'; // switch color
    GetStr (t)[1] = charmap.black_down_triangle; // '▼'
    AppendString ((String *)&s, t, 255);
    ShowVCStr (dest, x, y, (String *)&s, attr1, attr2);
  }

  DBG_LEAVE (); //EXIT //VScrollBar
  return pos;
}
