// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

String *remove_spaces_right (String *s)
{
  size_t len = Length (s);

  while ((len != 0) && (GetStr (s)[len - 1] == ' '))
  {
    len--;
    SetLength (s, len);
  }

  return s;
}
