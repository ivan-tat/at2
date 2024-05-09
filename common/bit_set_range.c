// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

void bit_set_range (uint8_t *s, uint32_t start, uint32_t end) {
  size_t a0, a1; // address
  uint8_t b0, b1; // `OR' bit-mask (to set 1 bits)

  if (start <= end) {
    a0 = start;
    a1 = end;
  } else {
    a0 = end;
    a1 = start;
  }

  a0 = a0 / 8;
  b0 = 0xFF << (a0 % 8);
  //    76543210
  // 0: 11111111
  // 1: 11111110
  // 2: 11111100
  // 3: 11111000
  // 4: 11110000
  // 5: 11100000
  // 6: 11000000
  // 7: 10000000
  a1 = a1 / 8;
  b1 = ~(0xFE << (a1 % 8));
  //    76543210
  // 0: 00000001
  // 1: 00000011
  // 2: 00000111
  // 3: 00001111
  // 4: 00011111
  // 5: 00111111
  // 6: 01111111
  // 7: 11111111

  if (a0 == a1)
    s[a0] |= b0 & b1;
  else {
    s[a0] |= b0;

    if (a1 - a0 - 1)
      memset (s + a0 + 1, 0xFF, a1 - a0 - 1);

    s[a1] |= b1;
  }
}
