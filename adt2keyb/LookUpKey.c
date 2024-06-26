// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

bool LookUpKey (ExtKeyCode key, const ExtKeyCode *table, size_t size) {
  while (size) {
    if (*table == key)
      return true;
    table++;
    size--;
  }

  return false;
}
