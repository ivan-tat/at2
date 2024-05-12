// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

bool IsWild (const String *str, const String *wilds, bool ignore_case) {
  String_t s, w; // Local copy of `str' and `wilds'
  size_t sc, wc; // Counter for positions in `s' and `w'
  size_t sl, wl; // Length of `s' and `w'

  if ((Length (str) == Length (wilds))
  &&  (memcmp (GetStr (str), GetStr (wilds), Length (str)) == 0))
    return true;

  { // Replace multiple continuous `*' in `w' with single one
    bool asterisk = false;
    size_t count = Length (wilds);
    const unsigned char *src = GetStr (wilds);
    unsigned char *dst = w.str;

    while (count) {
      if (*src == '*') {
        if (!asterisk) {
          *dst = *src;
          dst++;
          asterisk = true;
        }
      } else {
        *dst = *src;
        dst++;
        asterisk = false;
      }
      src++;
      count--;
    }

    w.len = dst - w.str;
  }

  // For fast end, if `w' is only '*'
  if ((w.len == 1) && (w.str[0] == '*'))
    return true;

  CopyString ((String *) &s, str, 255);

  sl = s.len;
  wl = w.len;
  if ((sl == 0) || (wl == 0))
    return false;

  if (ignore_case) { // Upper case all letters
    s = Upper ((String *) &s);
    w = Upper ((String *) &w);
  }

  sc = 0;
  wc = 0;
  do {
    if (s.str[sc] == w.str[wc]) { // Equal letters
      // Go to next letter
      sc++;
      wc++;
      continue;
    }
    if (w.str[wc] == '?') { // Equal to '?'
      // Go to next letter
      sc++;
      wc++;
      continue;
    }
    if (w.str[wc] == '*') { // Handling of '*'
      String_t h, t;
      size_t hl, i;

      h = Copy ((String *) &w, wc + 2, wl);

      // Search next
      i = PosChr ('*', (String *) &h);
      if (i)
        h.len = i - 1;

      hl = h.len;
      if (i == 0) { // No '*' in the rest, compare the ends
        if (h.len == 0) // '*' is the last letter
          return true;
        // Check the rest for equal Length and no '?'
        for (i = 0; i < hl; i++)
          if ((h.str[hl - i - 1] != '?')
          &&  (h.str[hl - i - 1] != s.str[sl - i - 1]))
            return false;

        return true;
      }

      // Handle all to the next '*'
      wc += 1 + hl;

      t = Copy ((String *) &s, sc + 1, 255);
      i = FindPart ((String *) &h, (String *) &t);

      if (i == 0)
        return false;

      sc = i + hl;
      continue;
    }

    return false;

  } while ((sc < sl) && (wc < wl));

  // No completed evaluation
  return (sc >= sl) && ((wc >= wl) || (w.str[wl - 1] == '*'));
}
