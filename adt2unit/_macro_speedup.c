// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

uint16_t _macro_speedup (void) {
  return (macro_speedup > 0) ? macro_speedup : (macro_speedup + 1);
}
