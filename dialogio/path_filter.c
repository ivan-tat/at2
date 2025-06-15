// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static String *path_filter (String *dst, const String *path)
{
  size_t len = Length (path);

  if ((len > 3) && (GetStr (path)[len - 1] == PATHSEP))
  {
    CopyString (dst, path, len);
    Delete (dst, len, 1);
  }
  else
  {
    String_t s = Upper_filename (path);
    CopyString (dst, (String *)&s, len);
  }

  return dst;
}
