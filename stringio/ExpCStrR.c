// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

String_t ExpCStrR (const String *str, uint8_t len, char chr)
{
  String_t r;
  size_t size = Length (str), clen = CStrLen (str);

  if (size)
    memcpy (r.str, GetStr (str), size);

  if (len > clen)
  {
    size_t n = len - clen;

    if (size + n > 255) n = 255 - size; // Prevent target buffer overrun

    if (n)
    {
      memset (r.str + size, chr, n);
      size += n;
    }
  }

  r.len = size;

  return r;
}
