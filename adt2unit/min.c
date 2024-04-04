// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

int32_t min (int32_t value, int32_t minimum) {
  if (value >= minimum)
    return value;
  else
    return minimum;
}
