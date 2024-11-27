// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

__inline__ void Mouse_SetPos (uint16_t x, uint16_t y)
{
  uint16_t a = 4; // Set mouse pointer position
  uint32_t c = x, d = y; // force the use of `movzx' opcode to initialize

  __asm__ __volatile__
  (
    "int $0x33"
    :
    : "a" (a), "c" (c), "d" (d)
    : "ebx", "esi", "edi", "cc", "memory"
  );
}
