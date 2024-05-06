// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Return value: 0 to max for current video mode
static __inline__ uint8_t BDA_get_active_video_page (void) {
  return _farnspeekb (0x462); // Active display page number
}
