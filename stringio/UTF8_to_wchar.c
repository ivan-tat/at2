// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#if (__SIZEOF_WCHAR_T__ != 2) && (__SIZEOF_WCHAR_T__ != 4)
#error Unsupported "wchar_t" size
#endif // (__SIZEOF_WCHAR_T__ != 2) && (__SIZEOF_WCHAR_T__ != 4)

__inline__ wchar_t UTF8_to_wchar (const char **pstr) {
  const unsigned char *s = (const unsigned char *) *pstr;
  uint32_t c = *s;

  s++;
  if (c & 0x80) {
    if ((c & 0xE0) == 0xC0) { // 2 bytes, 0..0x7FF
      c = (c & 0x1F) << 6;

      if ((*s & 0xC0) == 0x80) {
        c |= (*s & 0x3F);
        s++;
      } else
        c = '?';
    } else if ((c & 0xF0) == 0xE0) { // 3 bytes, 0x800..0xFFFF
      c = (c & 0x0F) << 12;

      if ((*s & 0xC0) == 0x80) {
        c |= (*s & 0x3F) << 6;
        s++;

        if ((*s & 0xC0) == 0x80) {
          c |= (*s & 0x3F);
          s++;
        } else
          c = '?';
      } else
        c = '?';
    } else if ((c & 0xF8) == 0xF0) { // 4 bytes, 0x10000..0x10FFFF
      c = (c & 0x07) << 18;

      if ((*s & 0xC0) == 0x80) {
        c |= (*s & 0x3F) << 12;
        s++;

        if ((*s & 0xC0) == 0x80) {
          c |= (*s & 0x3F) << 6;
          s++;

          if ((*s & 0xC0) == 0x80) {
            c |= (*s & 0x3F);
            s++;
            if ((sizeof (wchar_t) == 2) && (c >= 0x10000))
              c = '?';
          } else
            c = '?';
        } else
          c = '?';
      } else
        c = '?';
    } else
      c = '?';
  }

  *pstr = (const char *) s;
  return c;
}
