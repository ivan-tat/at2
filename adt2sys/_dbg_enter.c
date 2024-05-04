// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void _dbg_enter (const char *filename, const char *funcname) {
  char *l = (char *) GetStr (_last_debug_str_);
  char *d = (char *) GetStr (_debug_str_);
  size_t len = Min (sizeof (_last_debug_str_), sizeof (_debug_str_)) - 1;

  strncpy (l, d, len);
  SetLength (_last_debug_str_, strnlen (l, len));
  strncpy (d, filename, sizeof (_debug_str_) - 1);
  strncat (d, ":", sizeof (_debug_str_) - 1);
  strncat (d, funcname, sizeof (_debug_str_) - 1);
  SetLength (_debug_str_, strnlen (d, sizeof (_debug_str_) - 1));
}
