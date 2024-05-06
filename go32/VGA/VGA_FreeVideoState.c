// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void VGA_FreeVideoState (VGAVideoState_t *vs) {
  if (vs->data) {
    free (vs->data);
    vs->data = NULL;
    vs->data_size = 0;
  }
  if (vs->screen) {
    free (vs->screen);
    vs->screen = NULL;
  }
}
