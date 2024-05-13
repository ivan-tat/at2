// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

String_t BaseNameOnly (const String *path) {
  String_t r;
  String p[MAX_PATH_LEN + 1]; // Local `path' copy
  ssize_t de /* Dir. end */, es /* Ext. start */, len;

  CopyString ((String *) &p, path, MAX_PATH_LEN);

  // Allow slash and backslash
  DoDirSeparators (p);

  _FSplit (p, &de, &es);

  len = es - de - 1;
  if (len > MAX_NAME_LEN)
    len = MAX_NAME_LEN;

  r = Copy (p, de + 1, len);

  return r;
}
