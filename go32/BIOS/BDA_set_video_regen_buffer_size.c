// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static __inline__ void BDA_set_video_regen_buffer_size (uint16_t size) {
  _farnspokew (0x44C, size); // Video regen buffer length in bytes
}
