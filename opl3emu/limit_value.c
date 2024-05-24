// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static int32_t limit_value (int32_t value, int32_t lo_bound,
                            int32_t hi_bound) {

  return (value > hi_bound) ? hi_bound
                            : ((value < lo_bound) ? lo_bound
                                                  : value);
}
