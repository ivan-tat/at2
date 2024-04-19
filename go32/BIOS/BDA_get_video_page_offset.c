// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static __inline__ uint16_t BDA_get_video_page_offset (void) {
  return _farnspeekw (0x44E); // Offset of current video page in video
                              // regen buffer
}
