// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void fib_file_loader_restore (uint8_t xstart, uint8_t ystart)
{
  DBG_ENTER ("fib_file_loader_restore");

  move_to_screen_data = ptr_screen_backup;
  move_to_screen_area[0] = xstart;
  move_to_screen_area[1] = ystart;
  move_to_screen_area[2] = xstart + 43 + 2 + 1;
  move_to_screen_area[3] = ystart + 3 + 1;
  move2screen ();

  DBG_LEAVE (); //EXIT //fib_file_loader_restore
}
