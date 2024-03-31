// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void Frame (tSCREEN_MEM *dest, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
            uint8_t attr1, const String *title, uint8_t attr2,
            const String *border) {

  void *p; // output screen address
  int_least8_t xexp1, xexp2, xexp3, yexp1, yexp2;
  uint_least8_t y, n;
  bool wide_range_type;
  bool shadow_enabled;

  wide_range_type = fr_setting.wide_range_type;
  shadow_enabled = fr_setting.shadow_enabled;
  if (fr_setting.update_area) {
    area_x1 = x1;
    area_y1 = y1;
    area_x2 = x2;
    area_y2 = y2;
  }

  if (wide_range_type) {
    xexp1 = 4;
    xexp2 = -1;
    xexp3 = 7;
    yexp1 = 1;
    yexp2 = 2;
    DupChar (x1-3, y1-1, ' ', attr1, x2-x1+7, dest);
    DupChar (x1-3, y2+1, ' ', attr1, x2-x1+7, dest);
    y = y1;
    do {
      DupChar (x1-3, y, ' ', attr1, 3, dest);
      DupChar (x2+1, y, ' ', attr1, 3, dest);
      y++;
    } while (y <= y2);
  } else {
    xexp1 = 1;
    xexp2 = 2;
    xexp3 = 1;
    yexp1 = 0;
    yexp2 = 1;
  }

  n = x2-x1-1;
  DupChar (x1,   y1, border[1], attr1, 1, dest);
  DupChar (x1+1, y1, border[2], attr1, n, dest);
  DupChar (x2,   y1, border[3], attr1, 1, dest);

  y = y1;
  do {
    y++;
    DupChar (x1,   y, border[4], attr1, 1, dest);
    DupChar (x1+1, y, ' ',       attr1, n, dest);
    DupChar (x2,   y, border[5], attr1, 1, dest);
  } while (y < y2);

  DupChar (x1,   y2, border[6], attr1, 1, dest);
  DupChar (x1+1, y2, border[7], attr1, n, dest);
  DupChar (x2,   y2, border[8], attr1, 1, dest);

  n = Length (title);
  if (n) {
    const unsigned char *s; // input string
    uint_least16_t a; // prepared attribute
    uint8_t x;

    x = x2-x1-n;
    x = ((uint16_t) x + 1) >> 1;
    PosChar (x1+x, y1); // set `absolute_pos'
    p = ((char *) dest) + absolute_pos;
    s = GetStr (title);
    a = attr2 << 8;
    do {
      *((uint16_t *) p) = *s | a;
      s++;
      p = ((uint16_t *) p) + 1;
    } while (--n);
  }

  if (shadow_enabled) {
    y = y1-yexp1;
    do {
      y++;
      PosChar (x2+xexp1, y); // set `absolute_pos'
      p = ((char *) dest) + absolute_pos;
      *((uint8_t *) p + 1) = 0x07;
      *((uint8_t *) p + 3) = 0x07;
      p = ((uint8_t *) p) + 5;
      if (MaxCol > 180) {
        *((uint8_t *) p + 1) = 0x07;
        p = ((uint16_t *) p) + 1;
      }
    } while (y <= y2);

    PosChar (x1+xexp2, y2+yexp2); // set `absolute_pos'
    p = ((char *) dest) + absolute_pos + 1;
    n = x2-x1+xexp3;
    if (MaxLn >= 60)
      n--;
    do {
      *((uint8_t *) p) = 0x07;
      p = ((uint16_t *) p) + 1;
    } while (--n);
  }
}
