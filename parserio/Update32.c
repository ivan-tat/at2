// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

uint32_t Update32 (const void *buf, size_t size, uint32_t CRC)
{
  for (; size; size--)
    CRC = CRC32_table[(CRC & 0xFF) ^ (*(uint8_t *)buf++)] ^ (CRC >> 8);

  return CRC;
}
