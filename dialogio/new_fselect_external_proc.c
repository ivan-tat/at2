// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void new_fselect_external_proc (void)
{
  CopyString (mn_environment.curr_item, _fs.fstream.stuff[mn_environment.curr_pos - 1].name, 255);
  if (old_fselect_external_proc != NULL) old_fselect_external_proc ();
}
