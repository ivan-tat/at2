// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

uint8_t bar_height          = BAR_HEIGHT_MAX;
uint8_t bar_subdivs         = 16;
uint8_t bar_char_none       = '\xE0';
uint8_t bar_char_tail_first = '\xE1';
uint8_t bar_char_full       = '\xDB';

#include "realtime/show_bar.c"
#include "realtime/decay_bars_refresh.c"
