// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

uint8_t vscroll_bar (uint8_t x, uint8_t y, uint8_t size,
                     uint16_t len1, uint16_t len2, uint16_t pos,
                     uint8_t attr1, uint8_t attr2)
{
  uint8_t temp;

  if (size > work_MaxLn - y)
    size = work_MaxLn - y;
  if (size < 5)
    size = 5;

  if (size - 2 - 1 < 10)
    temp = _partial (len1, len2, size - 2 - 1);
  else
    temp = _partial (len1, len2, size - 2 - 1 - 2);

  if ((pos == temp) && !force_scrollbars)
    return temp; //vscroll_bar

  if ((size < len1 * 4) && (len1 > 5))
  {
    String_t s, t;

    pos = temp;

    s.len = 0;
    t = ExpStrL ((String *)&s, size - 2, '\xB0');
    s.len = 1;
    s.str[0] = '\x1E';
    AppendString ((String *)&s, (String *)&t, 255);
    t.len = 1;
    t.str[0] = '\x1F';
    AppendString ((String *)&s, (String *)&t, 255);
    show_vstr (x, y, (String *)&s, attr1);

    if (size - 2 - 1 < 10)
    {
      s.len = 1;
      s.str[0] = '\xB2';
    }
    else
    {
      s.len = 3;
      s.str[0] = '\xB2';
      s.str[1] = '\xB2';
      s.str[2] = '\xB2';
    }
    show_vstr (x, y + 1 + temp, (String *)&s, attr2);
  }
  else
  {
    String_t s, t;

    s.len = 0;
    t = ExpStrL ((String *)&s, size - 2, '\xB1');
    s.len = 1;
    s.str[0] = '\x1E';
    AppendString ((String *)&s, (String *)&t, 255);
    t.len = 1;
    t.str[0] = '\x1F';
    AppendString ((String *)&s, (String *)&t, 255);
    show_vstr (x, y, (String *)&s, attr1);
  }

  return pos; //EXIT //vscroll_bar
}
