// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2026 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

void bit_assign (uint8_t *s, uint32_t index, bool bit) {
  s[index / 8] = bit ?  s[index / 8] |  (1 << (index % 8))
                     : (s[index / 8] & ~(1 << (index % 8)));
}
