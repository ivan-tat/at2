// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void progress_window_update (progress_callback_t *cb, int32_t value, int32_t refresh_diff)
{
  progress_window_t *self = (progress_window_t *)cb;
  bool f_msg, f_num_steps, f_step, f_value, f_old_value;

  cb->msg[sizeof (cb->msg) - 1] = '\0'; // terminate string
  f_msg = strcmp (self->last_msg, cb->msg) != 0;
  f_num_steps = self->last_num_steps != cb->num_steps;
  f_step = self->last_step != cb->step;
  f_value = self->last_value != cb->value;
  f_old_value = self->last_old_value != cb->old_value;

  if (f_msg)
  {
    String s[sizeof (self->last_msg)];
    String_t msg;
    size_t maxlen, len;

    strncpy (self->last_msg, cb->msg, sizeof (self->last_msg) - 1);

    StrToString (s, self->last_msg, sizeof (s) - 1);
    maxlen = self->hsize - 4 - 5;
    len = Length (s);
    if (len < maxlen)
    {
      String_t t = iCASE (s);
      msg = ExpStrR ((String *)&t, maxlen, ' ');
    }
    else
    {
      if (len > maxlen) SetLength (s, maxlen);
      msg = iCASE (s);
    }

    ShowCStr (screen_ptr, self->xstart + 2, self->ystart + 1,
              (String *)&msg,
              dialog_background + dialog_text,
              dialog_background + dialog_hi_text);
  }
  if (f_num_steps)
  {
    self->last_num_steps = cb->num_steps;
    progress_num_steps = cb->num_steps;
  }
  if (f_step)
  {
    self->last_step = cb->step;
    progress_step = cb->step;
  }
  if (f_value)
  {
    self->last_value = cb->value;
    progress_value = cb->value;
  }
  if (f_old_value)
  {
    self->last_old_value = cb->old_value;
    progress_old_value = cb->old_value;
  }

  if (self->opened)
    show_progress2 (value, refresh_diff);
}
