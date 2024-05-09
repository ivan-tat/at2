// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

int32_t Str2num (const String *str, uint8_t base) {
  int32_t value = 0;

  if ((base == 2) || (base == 10) || (base == 16)) {
    size_t len = Length (str);
    const unsigned char *s = GetStr (str);

    while (len) {
      unsigned char c = UpCase (*s);

      s++;

      if ((c >= '0') && (c <= '9'))
        c -= '0';
      else if ((c >= 'A') && (c <= 'F'))
        c -= 'A' - 10;
      else
        c = 0;
      if (c >= base)
        c = 0;

      value = value * base + c;
      len--;
    }
  }

  return value;
}
