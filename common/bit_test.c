// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

bool bit_test (const uint8_t *s, uint32_t value) {
  return (s[value / 8] & (1 << (value % 8))) ? true : false;
}
