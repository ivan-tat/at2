// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_patterns_cff (tFIXED_SONGDATA *song,
                              cff_patterns_t *data, size_t size, unsigned patterns)
{
  pat_event_cff_t *src_event;
  size_t offset;
  unsigned pos, idx;
  uint8_t processed[(255+7)/8];
  play_state_cff_t state;

  src_event = &(*data)[0][0][0];
  offset = 0;
  for (unsigned pat = 0; pat < PATTERNS_MAX_CFF; pat++)
  {
    tRAW_PATTERN_DATA *pat_data = get_pattern_data (song, pat);

    for (unsigned line = 0; line < PATTERN_LEN_CFF; line++)
      for (unsigned chan = 0; chan < CHANNELS_MAX_CFF; chan++)
      {
        tCHUNK dst_event;
        pat_event_cff_t event;

        if (offset + sizeof (*src_event) <= size)
        {
          event = *src_event;
          src_event++;
          offset += sizeof (*src_event);
        }
        else if (offset < size)
        {
          memset (&event, 0, sizeof (event));
          memcpy (&event, src_event, size - offset);
          //src_event++; // not needed
          offset = size;
        }
        else
          break;

        if (!is_empty_event_cff (&event))
        {
          get_pat_event_cff (&dst_event, &event);
          (*pat_data)[chan][line] = dst_event;
        }
      }
  }

  memset (processed, 0, sizeof (processed));
  if (!accurate_conv)
    for (unsigned chan = 0; chan < CHANNELS_MAX_CFF; chan++)
      state.ins[chan] = chan + 1;
  else
    memset (state.ins, 0, sizeof (state.ins));
  memset (state.arp_param, 0, sizeof (state.arp_param));

  pos = 0;
  idx = patterns; // acts as stop
  do
  {
    if (song->pattern_order[pos] < PATTERNS_MAX_CFF)
    {
      tRAW_PATTERN_DATA *pat_data;

      idx = song->pattern_order[pos];
      pat_data = get_pattern_data (song, idx);
      src_event = &(*data)[idx][0][0];
      offset = sizeof (pat_data_cff_t) * idx;
      state.pat_break = PATTERN_LEN;
      for (unsigned line = 0; line < PATTERN_LEN_CFF; line++)
      {
        state.line = line;

        for (unsigned chan = 0; chan < CHANNELS_MAX_CFF; chan++)
        {
          tCHUNK dst_event = (*pat_data)[chan][line];
          pat_event_cff_t event;

          if (offset + sizeof (*src_event) <= size)
          {
            event = *src_event;
            src_event++;
            offset += sizeof (*src_event);
          }
          else if (offset < size)
          {
            memset (&event, 0, sizeof (event));
            memcpy (&event, src_event, size - offset);
            //src_event++; // not needed
            offset = size;
          }
          else
            break;

          state.chan = chan;

          if (   update_pat_event_cff (&dst_event, &state, &event)
              && !bit_test (processed, idx))
            (*pat_data)[chan][line] = dst_event;
        }
      }

      bit_set (processed, idx);
    }

    pos++;
  } while ((idx < patterns) && (pos < PATTERN_ORDER_LEN_CFF));
}
