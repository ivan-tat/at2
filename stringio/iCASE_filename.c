// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

String_t iCASE_filename (const String *str) {
  String_t r;

#if linux||unix
  CopyString ((String *) &r, str, 255);
#else // !(linux||unix)
  r = iCASE (str);
#endif // !(linux||unix)

  return r;
}
