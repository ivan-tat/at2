// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

uint16_t Update16 (const uint8_t *buf, size_t size, uint16_t CRC) {
  while (size) {
    CRC = CRC16_table[(CRC & 0xFF) ^ *buf] ^ (CRC >> 8);
    buf++;
    size--;
  }

  return CRC;
}
