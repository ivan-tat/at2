// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

AT2_char_t *UTF8str_to_AT2 (AT2_char_t *dest, const char *s) {
  AT2_char_t *p = (AT2_char_t *) dest;
  wchar_t c;

  do {
    c = UTF8_to_wchar (&s);
#if __SIZEOF_WCHAR_T__ == 4
    if (c >= UCS2_CP_SIZE)
      c = '?';
#endif // __SIZEOF_WCHAR_T__ == 4
    *p = UCS2_to_AT2_ct[c];
    p++;
  } while (c);

  return dest;
}
