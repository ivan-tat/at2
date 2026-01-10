// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Saved from "iloaders.inc"

String_t truncate_string (const String *s)
{
  String_t r;

  CopyString ((String *)&r, s, 255);
  while (   (r.len > 0)
         && (   (r.str[r.len - 1] == '\0')
             || (r.str[r.len - 1] == '\x20')
             || (r.str[r.len - 1] == (uint8_t)'\xFF')))
    r.len--;

  return r;
}
