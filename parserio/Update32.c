// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

uint32_t Update32 (const uint8_t *buf, size_t size, uint32_t CRC) {
  while (size) {
    CRC = CRC32_table[(CRC & 0xFF) ^ *buf] ^ (CRC >> 8);
    buf++;
    size--;
  }

  return CRC;
}
