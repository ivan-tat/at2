// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

String_t DietStr (const String *str, uint8_t size) {
  String_t r;
  size_t len = Length (str);

  if (len <= size)
    CopyString ((String *) &r, str, len);
  else {
    size_t head = (size - 3) / 2, tail = size - head - 3;
    const unsigned char *s = GetStr (str);

    r.len = size;

    if (head)
      memcpy (r.str, s, head);

    r.str[head + 0] = '.';
    r.str[head + 1] = '.';
    r.str[head + 2] = '.';

    if (tail)
      memcpy (r.str + head + 3, s + len - tail, tail);
  }

  return r;
}
