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
  p += MaxCol; /* vertical step */                                           \
  i++;

#define LOOP_TAIL_Y                                                          \
  LOOP_TAIL                                                                  \
  y++; // vertical step

void show_vstr (uint8_t xpos, uint8_t ypos, const String *str, uint8_t attr) {

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

    if ((x < area_x1) || (x > area_x2+2)) {
      while (1) {
        *p = *s | a; // normal

        LOOP_TAIL
      }
    } else if (x <= area_x1+1) {
      while (1) {
        if ((y < area_y1) || (y > area_y2))
          *p = *s | a; // normal

        LOOP_TAIL_Y
      }
    } else if (x <= area_x2) {
      uint_least8_t y21 = area_y2+1; // shadow: bottom line's Y

      while (1) {
        if ((y < area_y1) || (y > y21))
          *p = *s | a; // normal
        else if (y > area_y2)
          *((unsigned char *) p) = *s; // shadow

        LOOP_TAIL_Y
      }
    } else {
      uint_least8_t y11 = area_y1+1; // shadow: vertical rectangle's start Y
      uint_least8_t y21 = area_y2+1; // shadow: vertical rectangle's end Y

      while (1) {
        if ((y < y11) && (y > y21))
          *p = *s | a; // normal
        else
          *((unsigned char *) p) = *s; // shadow

        LOOP_TAIL_Y
      }
    }
  }
}

#undef LOOP_TAIL
#undef LOOP_TAIL_Y
