// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

int SetCustomVideoMode (tCUSTOM_VIDEO_MODE vmode) {
  if (vmode >= (sizeof (VGACustomTextModes) / sizeof (VGACustomTextModes[0])))
    return -1;

  VGA_MakeCustomTextMode (&VGACustomTextModes[vmode]);
  OnInitVideoMode (true);

  return 0;
}
