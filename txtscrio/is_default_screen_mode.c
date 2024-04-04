// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

bool is_default_screen_mode (void) {
#if GO32
  return (!program_screen_mode)
         || ((program_screen_mode == 3) && (comp_text_mode < 4));
#else // !GO32
  return !program_screen_mode;
#endif // !GO32
}
