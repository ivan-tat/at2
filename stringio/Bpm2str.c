// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

String_t Bpm2str (double bpm) {
  String_t r;

  if (bpm < 1000) {
    uint_least16_t num = 10.0 * bpm;

    r = Num2str (num / 10, 10);
    r.str[r.len + 0] = '.';
    r.str[r.len + 1] = '0' + (num % 10);
    r.len += 2;
  } else
    r = Num2str (bpm, 10);

  return r;
}
