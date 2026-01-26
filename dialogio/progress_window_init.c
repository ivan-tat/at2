// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void progress_window_init (progress_window_t *self, uint8_t hsize, uint8_t vsize,
                           const String *title, const String *hint)
{
  self->opened = true;
  self->hsize = hsize;
  self->vsize = vsize;
  self->last_msg[0] = '\0';
  self->last_num_steps = 1;
  self->last_step = 1;
  self->last_value = 1;
  self->last_old_value = UINT8_NULL;
  self->callback.msg[0] = '\0';
  self->callback.num_steps = self->last_num_steps;
  self->callback.step = self->last_step;
  self->callback.value = self->last_value;
  self->callback.old_value = self->last_old_value;
  self->callback.update = progress_window_update;

  ScreenMemCopy (screen_ptr, ptr_screen_backup);

  centered_frame_vdest = screen_ptr;
  centered_frame (&self->xstart, &self->ystart, hsize - 1, vsize - 1,
                  title,
                  dialog_background + dialog_border,
                  dialog_background + dialog_title,
                  frame_double);

  if ((hint != NULL) && (Length (hint) != 0))
    ShowStr (screen_ptr,
             self->xstart + hsize - Length (hint) - 1,
             self->ystart + vsize - 1,
             hint,
             dialog_background + dialog_border);

  progress_xstart = self->xstart + 2;
  progress_ystart = self->ystart + vsize - 2;
  progress_num_steps = self->last_num_steps;
  progress_step = self->last_step;
  progress_value = self->last_value;
  progress_old_value = self->last_old_value;
}
