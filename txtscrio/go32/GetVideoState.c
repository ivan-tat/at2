// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

int GetVideoState (VGAVideoState_t *vs, bool save_screen) {
  return VGA_SaveVideoState (vs, save_screen);
}
