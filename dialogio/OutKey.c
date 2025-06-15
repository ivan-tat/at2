// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static char OutKey (const String *str)
{
  ssize_t l = Length (str); // size_t
  ssize_t i = PosChr ('~', str) - 1;

  return (l >= 3) && (i >= 0) && (i < l - 2) && (GetStr (str)[i + 2] == '~') ? GetStr (str)[i + 1] : '~';
}
