// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void progress_window_close (progress_window_t *self)
{
  if (self->opened)
  {
    move_to_screen_data = ptr_screen_backup;
    move_to_screen_area[0] = self->xstart;
    move_to_screen_area[1] = self->ystart;
    move_to_screen_area[2] = self->xstart + self->hsize + 2; // +2 for shadow
    move_to_screen_area[3] = self->ystart + self->vsize + 1; // +1 for shadow
    move2screen ();
    self->opened = false;
  }
}
