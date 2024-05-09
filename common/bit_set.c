// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

void bit_set (uint8_t *s, uint32_t value) {
  s[value / 8] |= 1 << (value % 8);
}
