// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static bool _is_status_window_visible (void)
{
  return ((area_x1 == 0) && (area_y1 == 0)) || (area_x1 > 20) || (area_y1 > 6);
}
