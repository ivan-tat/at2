// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

void bit_clear_range (uint8_t *s, uint32_t start, uint32_t end) {
  size_t a0, a1; // address
  uint8_t b0, b1; // `AND' bit-mask (to clear 0 bits)

  if (start <= end) {
    a0 = start;
    a1 = end;
  } else {
    a0 = end;
    a1 = start;
  }

  a0 = a0 / 8;
  b0 = ~(0xFF << (a0 % 8));
  //    76543210
  // 0: 00000000
  // 1: 00000001
  // 2: 00000011
  // 3: 00000111
  // 4: 00001111
  // 5: 00011111
  // 6: 00111111
  // 7: 01111111
  a1 = a1 / 8;
  b1 = 0xFE << (a1 % 8);
  //    76543210
  // 0: 11111110
  // 1: 11111100
  // 2: 11111000
  // 3: 11110000
  // 4: 11100000
  // 5: 11000000
  // 6: 10000000
  // 7: 00000000

  if (a0 == a1)
    s[a0] &= b0 | b1;
  else {
    s[a0] &= b0;

    if (a1 - a0 - 1)
      memset (s + a0 + 1, 0, a1 - a0 - 1);

    s[a1] &= b1;
  }
}
