// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

char *UTF8nstr_to_AT2 (char *dest, size_t size, const char *s) {
  unsigned char *p = (unsigned char *) dest;
  wchar_t c = 1;

  while (size && c) {
    c = UTF8_to_wchar (&s);
#if __SIZEOF_WCHAR_T__ == 4
    if (c >= 0x10000)
      c = '?';
#endif // __SIZEOF_WCHAR_T__ == 4
    *p = UCS2_to_AT2_ct[c];
    p++;
    size--;
  };

  return dest;
}
