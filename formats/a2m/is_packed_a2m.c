// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static bool is_packed_a2m (uint8_t version)
{
  return (version >= 1) && (version != 4) && (version != 8) && (version <= 14);
}
