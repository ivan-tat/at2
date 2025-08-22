// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void add_bank_position (const String *bank_name, int32_t bank_size, int32_t bank_position)
{
  String_t _bank_name;

  DBG_ENTER ("add_bank_position");

  {
    String_t t = Upper_filename (bank_name);
    _bank_name = CutStr ((String *)&t);
  }

  for (int32_t i = 0; i < bank_position_list_size; i++)
    if (       (!CompareStrings (bank_position_list[i].bank_name, (String *)&_bank_name))
        && (   (bank_position_list[i].bank_size == bank_size)
            || (bank_size == -1)))
    {
      bank_position_list[i].bank_position = bank_position;
      DBG_LEAVE (); //add_bank_position
      return;
    }

  if (bank_position_list_size < MAX_NUM_BANK_POSITIONS)
    bank_position_list_size++;
  else
  {
    bank_position_list_size = MAX_NUM_BANK_POSITIONS;
#if USE_FPC
    // HINT: GCC optimizes this with `memmove' from GLIBC but not with `custom_memmove'
    memmove (&bank_position_list[0], &bank_position_list[1],
             sizeof (bank_position_list[0]) * (bank_position_list_size - 1));
#else // !USE_FPC
    for (int32_t i = 0; i < bank_position_list_size - 1; i++)
      bank_position_list[i] = bank_position_list[i + 1];
#endif // !USE_FPC
  }

  CopyString (bank_position_list[bank_position_list_size - 1].bank_name, (String *)&_bank_name, BANK_NAME_LEN);
  bank_position_list[bank_position_list_size - 1].bank_size = bank_size;
  bank_position_list[bank_position_list_size - 1].bank_position = bank_position;

  DBG_LEAVE (); //EXIT //add_bank_position
}
