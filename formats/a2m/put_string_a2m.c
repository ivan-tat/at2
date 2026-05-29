// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In:
//   `dst_size': 1..256
//   `src_size': 1..256
//   `src_pos': start offset (from 0) in source string (excluding length byte)
//              If `src_pos' >= length of `src' string then the result will be an empty string.
//   `padding':
//      below 0 - no padding
//      '\0'..'\xFF' - fill with value
static void put_string_a2m (void *dst, uint16_t dst_size, const String *src, uint16_t src_size, uint8_t src_pos, int padding)
{
  uint8_t len = Length (src);

  if (len > src_size - 1)
    len = src_size - 1; // limit by input
  len = len > src_pos ? len - src_pos : 0;  // skip first `src_pos' characters
  if (len > dst_size - 1)
    len = dst_size - 1; // limit by output
  *((uint8_t *)dst) = len;
  if (len != 0)
    memcpy (&((uint8_t *)dst)[1], &GetStr (src)[src_pos], len); // skip first `src_pos' characters
  if ((padding >= 0) && (len < dst_size - 1))
    memset (&((uint8_t *)dst)[1 + len], padding, dst_size - 1 - len); // padding
}
