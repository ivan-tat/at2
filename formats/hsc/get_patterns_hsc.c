// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_patterns_hsc (tFIXED_SONGDATA *song,
                              const void *data, unsigned count, bool fix_note_bug)
{
  for (unsigned pat = 0; pat < PATTERNS_MAX_HSC; pat++)
    if (touch_pattern (song, pat))
    {
      tRAW_PATTERN_DATA *pat_data = get_pattern_data (song, pat);
      pat_event_hsc_t *event = &(*(patterns_hsc_t *)data)[pat][0][0];

      for (unsigned line = 0; line < PATTERN_LEN_HSC; line++)
        for (unsigned chan = 0; chan < CHANNELS_MAX_HSC; chan++)
        {
          if ((event->command != 0) || (event->param != 0))
            get_pat_event_hsc (&(*pat_data)[chan][line], *event, fix_note_bug);

          event++;
        }
    }

  fix_patterns_hsc (song, (const patterns_hsc_t *)data, count);
}
