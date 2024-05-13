// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

String_t ExtOnly (const String *path) {
  String_t r;
  String p[MAX_PATH_LEN + 1]; // Local `path' copy
  ssize_t de /* Dir. end */, es /* Ext. start */, len;

  CopyString ((String *) &p, path, MAX_PATH_LEN);

  // Allow slash and backslash
  DoDirSeparators (p);

  _FSplit (p, &de, &es);

  len = Length (p) - es; // Without leading '.'
  if (len > 0) {
    String_t t;

    if (len > MAX_EXT_LEN)
      len = MAX_EXT_LEN;

    t = Copy (p, es + 1, len);

    r = Lower_filename ((String *) &t);
  } else
    r.len = 0;

  return r;
}
