// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

bool is_in_block (uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
  if (block_xstart <= x1)
  {
    block_x0 = block_xstart;
    block_x1 = x1;
  }
  else
  {
    block_x0 = x1;
    block_x1 = block_xstart;
  }

  if (block_ystart <= y1)
  {
    block_y0 = block_ystart;
    block_y1 = y1;
  }
  else
  {
    block_y0 = y1;
    block_y1 = block_ystart;
  }

  return    (x0 >= block_x0)
         && (x0 <= block_x1)
         && (y0 >= block_y0)
         && (y0 <= block_y1);
}
