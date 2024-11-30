// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

uint8_t get_chanpos2 (const void *data, uint8_t channels, uint8_t scancode)
{
  const char *buf = data;
  uint8_t n = channels;
  bool f = true;

  while (n && f)
  {
    f = scancode == *buf;
    buf++;
    n--;
  }

  return f ? channels - n : 0;
}
