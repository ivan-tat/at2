// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void PosChar (uint8_t x, uint8_t y) {
  absolute_pos = ((y-1)*MaxCol + x-1) << 1;
}
