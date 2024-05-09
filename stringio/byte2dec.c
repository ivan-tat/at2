// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

String_t byte2dec (uint8_t value) {
  String_t r;

  if (value <= 99) {
    r.len = 2;
    r.str[0] = (value / 10) + '0';
    r.str[1] = (value % 10) + '0';
  } else {
    r.len = 3;
    r.str[0] = (value / 100) + '0';
    r.str[1] = ((value % 100) / 10) + '0';
    r.str[2] = ((value % 100) % 10) + '0';
  }

  return r;
}
