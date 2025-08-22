// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

int32_t get_bank_position (const String *bank_name, int32_t bank_size)
{
  int32_t r = 0;
  String_t _bank_name;

  DBG_ENTER ("get_bank_position");

  {
    String_t t = Upper_filename (bank_name);
    _bank_name = CutStr ((String *)&t);
  }

  for (int32_t i = 0; i < bank_position_list_size; i++)
    if (       (!CompareStrings (bank_position_list[i].bank_name, (String *)&_bank_name))
        && (   (bank_position_list[i].bank_size == bank_size)
            || (bank_size == -1)))
    {
      r = bank_position_list[i].bank_position;
      break;
    }

  DBG_LEAVE (); //EXIT //get_bank_position
  return r;
}
