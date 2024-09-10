// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

uint8_t scale_volume (uint8_t volume, uint8_t scale_factor) {
  double x = (63 - volume) * (63 - scale_factor) / 63.0;

  return 63 - x;
}
