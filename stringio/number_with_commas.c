// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// min: "0" - 1 character
// max: "4,294,967,295" - 13 characters
String *number_with_commas (String *dst, uint32_t num, uint8_t base)
{
  String_t s;
  size_t sl, dl, i;
  char *ps, *pd;

  s = Num2str (num, base);
  sl = s.len;
  dl = sl + ((sl - 1) / 3);
  SetLength (dst, dl);
  GetStr (dst)[dl - 1] = s.str [sl - 1];
  ps = (char *)&s.str[sl - 2];
  pd = (char *)&GetStr (dst)[dl - 2];
  i = 1;
  while (i < sl)
  {
    if ((i % 3) == 0)
    {
      *pd = ',';
      pd--;
    }
    *pd = *ps;
    ps--;
    pd--;
    i++;
  }

  return dst;
}
