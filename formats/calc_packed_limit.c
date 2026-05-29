// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// in case we cannot shrink data
static size_t calc_packed_limit (size_t size)
{
  return ((size > 128 * 8 / 9 ? size * 9 / 8 : 128) + 15) & ~(size_t)15; // +12.5% but at least +128 bytes
}
