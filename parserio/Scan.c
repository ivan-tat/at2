// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

int32_t Scan (const void *buf, ssize_t skip, ssize_t size,
              const String *str) {

  String _str[255+1];
  ssize_t len = Length (str), count = len;
  unsigned char *D = GetStr (_str);

  if (!len)
    return 0;

  CopyString (_str, str, 255);

  // Uppercase string
  do {
    unsigned char c = *D;

    if ((c >= 'a') && (c <= 'z'))
      c -= 'a' - 'A';

    *D = c;
    D++;
    count--;
  } while (count);

  count = size - skip - len + 1;
  if (count) {
    const unsigned char *S = (unsigned char *)buf + skip;
    D = GetStr (_str);

    do {
      ssize_t old_count;
      unsigned char a = *D & ~0x20, b;
      bool f;

      do {
        b = *S & ~0x20;
        S++;
        count--;
        f = a != b;
      } while (f && count);

      if (f)
        return -1;

      S--;
      old_count = count;
      count = len;

      do {
        f = false;
        while (count && !f) {
          f = *S == *D;
          S++;
          D++;
          count--;
        }

        if (f)
          return S - ((unsigned char *)buf + len);

        a = *(S - 1);
        if ((a >= 'a') && (a <= 'z'))
          a -= 'a' - 'A';
      } while (a == *(D - 1));

      S += count - len;
      D += count - len;
      count = old_count;
      S++;
    } while (S);
  }

  return -1;
}
