// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void trace_update_proc (void)
{
  DBG_ENTER ("trace_update_proc");

  space_pressed = scankey (SC_SPACE);

  if (play_status == isPlaying)
    if (   (_traceprc_last_order != current_order)
        || (_traceprc_last_pattern != current_pattern)
        || (_traceprc_last_line != current_line))
    {
      _traceprc_last_order = current_order;
      _traceprc_last_pattern = songdata.pattern_order[current_order];
      _traceprc_last_line = current_line;

      if (tracing)
      {
        if (!play_single_patt)
          pattern_patt = songdata.pattern_order[current_order];

        pattern_page = current_line;
        pattord_page = 0;
        pattord_hpos = 1;
        pattord_vpos = 1;

        while (current_order != (pattord_vpos + 4 * (pattord_hpos + pattord_page - 1) - 1))
          if (pattord_vpos < 4)
            pattord_vpos++;
          else if (pattord_hpos < MAX_ORDER_COLS)
          {
            pattord_hpos++;
            pattord_vpos = 1;
          }
          else if (pattord_page < (23 - (MAX_ORDER_COLS - 9)))
          {
            pattord_page++;
            pattord_vpos = 1;
          }
      }

      PATTERN_ORDER_page_refresh (pattord_page);
      PATTERN_page_refresh (pattern_page);
    }

  DBG_LEAVE (); //EXIT //trace_update_proc
}
