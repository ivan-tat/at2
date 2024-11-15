// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void count_order (uint8_t *entries)
{
  uint8_t index = 0, index2 = 0;

  DBG_ENTER ("count_order");

  do
  {
    if (songdata.pattern_order[index] != 0x80)
    {
      if (songdata.pattern_order[index] > 0x80)
      {
        if ((songdata.pattern_order[index] - 0x80) != index2)
        {
          index = songdata.pattern_order[index] - 0x80;
          index2 = index;
        }
        else
          break;
      }
    }
    else
      break;

    if (index < 0x80)
      index++;
  } while (index < 0x80);

  *entries = index;

  DBG_LEAVE (); //EXIT //count_order
}
