// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

String_t CutStr (const String *str) {
  String_t r;
  size_t len = Length (str);
  const unsigned char *s = GetStr (str);

  if (len) {
    size_t i = 0;

#if ADT2PLAY
    while ((i < len) && ((s[i] == '\0') || (s[i] == ' ')))
#else // !ADT2PLAY
    while ((i < len) && (s[i] == ' '))
#endif // !ADT2PLAY
      i++;

    if (i < len) {
      size_t j = len - 1;

#if ADT2PLAY
      while ((j > i) && ((s[j] == '\0') || (s[j] == ' ')))
#else // !ADT2PLAY
      while ((j > i) && (s[j] == ' '))
#endif // !ADT2PLAY
        j--;

      r.len = j - i + 1;
      memcpy (r.str, s + i, r.len);
    } else
      r.len = 0;
  } else
    r.len = 0;

  return r;
}
