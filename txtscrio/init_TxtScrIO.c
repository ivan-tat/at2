// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void init_TxtScrIO (void) {
#if GO32
  initialize ();
#endif // GO32
#if !ADT2PLAY
  memcpy (is_setting.char_filter, _valid_characters, sizeof (CharSet_t));
  memcpy (is_setting.valid_chars, _valid_characters, sizeof (CharSet_t));
  is_environment.locate_pos = 1;
  is_environment.min_num = 0;
  is_environment.max_num = UINT32_MAX;
  SetLength (is_environment.cur_str, 0);
  is_environment.ext_proc = NULL;
#endif // !ADT2PLAY
}
