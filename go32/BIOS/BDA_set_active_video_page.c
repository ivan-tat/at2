// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// page: 0 to max for current video mode
static __inline__ void BDA_set_active_video_page (uint8_t page) {
  _farnspokeb (0x462, page); // Active display page number
}
