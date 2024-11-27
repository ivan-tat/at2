// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

__inline__ void Mouse_GetPos (uint16_t *x, uint16_t *y)
{
  uint16_t a = 3; // Query mouse pointer position and button status
  uint16_t rc, rd;

  __asm__ __volatile__
  (
    "int $0x33"
    : "=c" (rc), "=d" (rd)
    : "a" (a)
    : "ebx", "esi", "edi", "cc", "memory"
  );

  *x = rc;
  *y = rd;
}
