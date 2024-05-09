// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

String_t Capitalize (const String *str) {
  String_t r;
  size_t len = Length (str);
  const unsigned char *s = GetStr (str);
  unsigned char *d = r.str;

  r.len = len;

  if (len) {
    unsigned char c = *s;

    s++;

    c = UpCase (c);

    *d = c;
    d++;

    do {
      unsigned char prev = c;

      c = *s;
      s++;

      if ((prev == ' ') && IsLower (c))
        c = UpCase (c);
      else if (IsUpper (c))
        c = LoCase (c);

      *d = c;
      d++;
      len--;
    } while (len);
  }

  return r;
}
