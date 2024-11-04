// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void centered_frame (uint8_t *xstart, uint8_t *ystart,
                     uint8_t hsize, uint8_t vsize, const String *name,
                     uint8_t attr1, uint8_t attr2, const String *border)
{
  *xstart = (work_MaxCol - hsize) / 2;
  *ystart = ((work_MaxLn - vsize) / 2) + ((work_MaxLn - vsize) % 2);

  Frame (centered_frame_vdest, *xstart, *ystart, *xstart + hsize, *ystart + vsize,
         attr1, name, attr2, border);
}
