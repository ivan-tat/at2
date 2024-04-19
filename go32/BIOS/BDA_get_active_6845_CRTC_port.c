// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static __inline__ uint16_t BDA_get_active_6845_CRTC_port (void) {
  return _farnspeekw (0x463); // Base port address for active
                              // 6845 CRT controller
                              // 3B4h = mono, 3D4h = color
}
