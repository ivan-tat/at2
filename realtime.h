// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

struct text_coord_t { uint8_t x, y; };

struct text_rect_t { struct text_coord_t top_left, bottom_right; };

#include "ui/status_window.h"
#include "ui/pattern_editor_window.h"
#include "ui/main_window.h"
#include "ui/decay_bars_window.h"
