// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

String_t PathOnly (const String *path) {
  String_t r;
  String p[MAX_PATH_LEN + 1]; // Local `path' copy
  ssize_t de /* Dir. end */, es /* Ext. start */;

  CopyString ((String *) &p, path, MAX_PATH_LEN);

  // Allow slash and backslash
  DoDirSeparators (p);

  _FSplit (p, &de, &es);

  if (de > MAX_DIR_LEN)
    de = MAX_DIR_LEN;

  r = Copy (p, 1, de);

  return r;
}
