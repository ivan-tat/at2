// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void FSplit (const String *path, String *dir, String *name, String *ext) {
  String p[MAX_PATH_LEN + 1]; // Local `path' copy
  ssize_t de /* Dir. end */, es /* Ext. start */, len;
  String_t t;

  CopyString ((String *) &p, path, MAX_PATH_LEN);

  // Allow slash and backslash
  DoDirSeparators (p);

  _FSplit (p, &de, &es);

  len = Length (p);

  t = Copy (p, 1, de);
  CopyString (dir, (String *) &t, MAX_DIR_LEN);

  t = Copy (p, de + 1, es - de - 1);
  CopyString (name, (String *) &t, MAX_NAME_LEN);

  t = Copy (p, es, len - es + 1);
  CopyString (ext, (String *) &t, MAX_EXT_LEN);
}
