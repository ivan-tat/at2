// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

int32_t get_bank_position (String *bank_name, int32_t bank_size)
{
  String_t s, t;

  DBG_ENTER ("get_bank_position");

  s = Upper_filename (bank_name);
  t = CutStr ((String *)&s);
  CopyString (bank_name, (String *)&t, 255);

  for (int32_t i = 0; i < bank_position_list_size; i++)
    if (       (!CompareStrings (bank_position_list[i].bank_name, bank_name))
        && (   (bank_position_list[i].bank_size == bank_size)
            || (bank_size == -1)))
    {
      DBG_LEAVE (); //get_bank_position
      return bank_position_list[i].bank_position;
    }

  DBG_LEAVE (); //EXIT //get_bank_position
  return 0;
}
