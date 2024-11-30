// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

uint8_t get_chanpos (const void *data, uint8_t channels, uint8_t scancode)
{
  uint8_t i = 0;

  do
  {
    const uint8_t *buf = (const uint8_t *)data + i;
    uint8_t n = channels - i;
    bool f = !n;

    while (n)
    {
      f = scancode == *buf;
      buf++;
      n--;
      if (!f)
        break;
    }

    if (!f)
      return 0;

    if (!is_4op_chan (channels - n))
      return channels - n;

    i++;
  }
  while (true);
}
