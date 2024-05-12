// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

bool SameName (const String *mask, const String *str) {
  String_t wilds;
  ssize_t ls = Length (str);

  CopyString ((String *) &wilds, mask, 255);

  if (wilds.len > ls) {
    ssize_t i = wilds.len - 1; // Position in `wilds'

    while ((i >= ls) && ((wilds.str[i] == '*') || (wilds.str[i] == '?')))
      i--;

    wilds.len = i + 1;
  }

  return IsWild (str, (String *) &wilds, false);
}
