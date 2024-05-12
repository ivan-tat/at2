// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

ssize_t FindPart (const String *wilds, const String *str) {
  if (PosChr ('?', wilds)) {
    ssize_t wl = Length (wilds), diff = Length (str) - wl, i = 0;

    while (i <= diff) {
      ssize_t j;

      for (j = 0; j < wl; j++) {
        if ((GetStr (wilds)[j] == GetStr (str)[i + j])
        ||  (GetStr (wilds)[j] == '?')) {
          if (j == wl - 1)
            return i + 1;
        } else
          break;
      }

      i++;
    }

    return 0;
  } else
    return Pos (wilds, str);
}
