// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#define LOOP_TAIL                                                            \
  if (i == n) /* check string end */                                         \
    break;                                                                   \
                                                                             \
  s++;                                                                       \
  p++;                                                                       \
  i++;

#define LOOP_TAIL_X                                                          \
  LOOP_TAIL                                                                  \
  x++;

void show_str (uint8_t xpos, uint8_t ypos, const String *str, uint8_t attr) {

  uint_least8_t n; // number of input characters left to proceed

  n = Length (str);
  if (n) {
    const unsigned char *s; // input string
    uint16_t *p; // output screen address
    uint_least16_t a; // prepared attribute
    uint_least8_t i; // index for a new character to show (1 based)
    uint_least8_t x, y; // character's screen coordinates

    s = GetString (str);
    a = attr << 8;
    x = xpos;
    y = ypos;
    p = ((uint16_t *) screen_ptr) + (ypos-1)*MaxCol + xpos-1;
    i = 1;

    if ((y < area_y1) || (y > area_y2+1)) {
      while (1) {
        *p = *s | a; // normal

        LOOP_TAIL
      }
    } else if (y == area_y1) {
      while (1) {
        if ((x < area_x1) || (x > area_x2))
          *p = *s | a; // normal

        LOOP_TAIL_X
      }
    } else if (y <= area_y2) {
      uint_least8_t x22 = area_x2+2; // shadow: vertical rectangle's end X

      while (1) {
        if ((x < area_x1) || (x > x22))
          *p = *s | a; // normal
        else if (x > area_x2)
          *((unsigned char *) p) = *s; // shadow

        LOOP_TAIL_X
      }
    } else {
      uint_least8_t x12 = area_x1+2; // shadow: bottom line's start X
      uint_least8_t x22 = area_x2+2; // shadow: bottom line's end X

      while (1) {
        if ((x < x12) && (x > x22))
          *p = *s | a; // normal
        else
          *((unsigned char *) p) = *s; // shadow

        LOOP_TAIL_X
      }
    }
  }
}

#undef LOOP_TAIL
#undef LOOP_TAIL_X
