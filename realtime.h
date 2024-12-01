// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#define BAR_HEIGHT_MAX 7

extern uint8_t bar_height;          // in characters
extern uint8_t bar_subdivs;         // depends on font
extern uint8_t bar_char_none;       // depends on font
extern uint8_t bar_char_tail_first; // depends on font
extern uint8_t bar_char_full;       // depends on font

void show_bar (uint8_t chan, uint8_t level, uint8_t xpos, uint8_t from_pos,
               uint8_t to_pos, uint8_t attr);
void decay_bars_refresh (void);
void status_refresh (void);
