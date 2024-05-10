// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

String_t CenterStr (const String *str, uint8_t size) {
  String_t r;
  size_t len = Length (str), head, tail;

  if (len < size) {
    head = (size - len) / 2;
    tail = size - len - head;
  } else {
    head = 0;
    tail = 0;
  }

  if (head)
    memset (r.str, ' ', head);

  memcpy (r.str + head, GetStr (str), len);

  if (tail)
    memset (r.str + head + len, ' ', tail);

  r.len = head + len + tail;

  return r;
}
