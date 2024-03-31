// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void ShowCStr (tSCREEN_MEM *dest, uint8_t x, uint8_t y, const String *str,
               uint8_t attr1, uint8_t attr2) {

  uint_least8_t n; // number of input characters left to proceed

  n = Length (str);
  if (n) {
    const unsigned char *s; // input string
    uint16_t *p; // output screen address
    uint_least16_t a1, a2; // prepared attributes

    s = GetStr (str);
    PosChar (x, y); // set `absolute_pos'
    p = (uint16_t *) (((char *) dest) + absolute_pos);
    a1 = attr1 << 8;
    a2 = attr2 << 8;
    do {
      unsigned char c; // character to show
      uint_least8_t flag; // 0=read, 1=stop, 2=show

      do {
        c = *s;
        s++;

        if (c == '~') { // color switch mark
          uint_least16_t t = a2;
          a2 = a1;
          a1 = t;
          n--;
          flag = n ? 0 : 1; // check string end
        } else
          flag = 2;

      } while (!flag); // color switch mark

      if (flag == 1)
        break;

      *p = c | a1;
      p++; // in 2-bytes units

    } while (--n); // check string end
  }
}
