// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void SetSize (uint16_t columns, uint16_t lines) {
  VGA_SetOffset (columns);
  VGA_SetTextModeDimensions (columns, lines, columns * lines * 2);
}
