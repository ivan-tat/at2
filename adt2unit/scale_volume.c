// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// volume: 0..63, scale_factor: 0..63
uint8_t scale_volume (uint8_t volume, uint8_t scale_factor) {
  return 63 - ((uint_least16_t)((63 - volume) * (63 - scale_factor) + 31) / 63);
}
