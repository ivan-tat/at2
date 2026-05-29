// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void next_saver_step (progress_callback_t *cb)
{
  cb->step++;
  cb->update (cb, -1, -1);
}
