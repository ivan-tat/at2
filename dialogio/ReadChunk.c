// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static String *ReadChunk (String *dst, const String *str, uint8_t pos)
{
  String s[255+1];
  ssize_t i;

  CopyString (s, str, 255); // make it local and modify it safely
  Delete (s, 1, pos - 1);
  i = PosChr ('$', s);
  if (i > 1)
    CopyString (dst, s, i - 1);
  else
    SetLength (dst, 0);

  return dst;
}
