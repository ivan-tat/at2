// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

int32_t max (int32_t value, int32_t maximum) {
  if (value <= maximum)
    return value;
  else
    return maximum;
}
