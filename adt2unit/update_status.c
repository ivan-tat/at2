// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void update_status (void)
{
  uint8_t x;
  String_t s;

  DBG_ENTER ("update_status");

  s = byte2hex (current_order);
  show_str (17, 3, (String *)&s, pattern_bckg + status_dynamic_txt);

  x = songdata.pattern_order[current_order];
  if (x >= 0x80)
    x = 0;
  s = byte2hex (x);
  show_str (20, 3, (String *)&s, pattern_bckg + status_dynamic_txt);

  show_str (17, 4, (String *)"\x02" "--", pattern_bckg + status_dynamic_txt);

#if !GO32
  _draw_screen_without_delay = true;
#endif // !GO32
  draw_screen ();

  DBG_LEAVE (); //EXIT //update_status
}
