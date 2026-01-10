// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static bool _1op_preview_active (void)
{
  uint_least8_t nm_slots = 0;

  for (uint_least8_t i = 0; i < 4; i++)
    if (_instedit_operator_enabled[i])
      nm_slots++;

  return nm_slots == 1;
}
