// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void update_mouse_position (void)
{
  if (mouse_active)
    Mouse_SetPos (scr_scroll_x, scr_scroll_y);
}
