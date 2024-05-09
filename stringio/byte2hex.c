// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

String_t byte2hex (uint8_t value) {
  String_t r;

  r.len = 2;
  r.str[0] = (value >> 4) + (((value >> 4) <= 9) ? '0' : 'A' - 10);
  r.str[1] = (value & 15) + (((value & 15) <= 9) ? '0' : 'A' - 10);

  return r;
}
