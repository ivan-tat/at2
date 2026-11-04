// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void fix_patterns_hsc (tFIXED_SONGDATA *song,
                              const patterns_hsc_t *data, unsigned count)
{
  uint8_t processed[(PATTERNS_MAX+7)/8];
  uint8_t voice[CHANNELS_MAX_HSC];

  memset (processed, 0, sizeof (processed));
  if (accurate_conv)
    memset (voice, INS_EMPTY, sizeof (voice));
  else
    for (unsigned chan = 0; chan < CHANNELS_MAX_HSC; chan++)
      voice[chan] = INS_PACK (chan);

  for (unsigned pos = 0; pos < lengthof (song->pattern_order, [0]); pos++)
  {
    unsigned pat = song->pattern_order[pos];

    if ((pat < PATTERNS_MAX_HSC) && touch_pattern (song, pat))
    {
      tRAW_PATTERN_DATA *pat_data = get_pattern_data (song, pat);
      const pat_event_hsc_t *event = &(*(patterns_hsc_t *)data)[pat][0][0];
      unsigned pat_break = PATTERN_LEN_HSC + 1;

      for (unsigned line = 0; line < PATTERN_LEN_HSC; line++)
        for (unsigned chan = 0; chan < CHANNELS_MAX_HSC; chan++)
        {
          tCHUNK a = (*pat_data)[chan][line];
          unsigned command = event->command, param = event->param;

          if (command & CMD_INS_FLAG_HSC)
          {
            if (line != pat_break)
            {
              voice[chan] = INS_PACK (param & 0x7F);
              if (!accurate_conv)
              {
                a.note      = NOTE_OFF;
                a.instr_def = voice[chan];
              }
            }
          }
          else
          {
            if (   (command >= CMD_NOTE_PACK_HSC (0))
                && (command <= CMD_NOTE_PACK_HSC (NOTE_MAX_HSC)))
            {
              if (accurate_conv)
              {
                if (voice[chan] == INS_EMPTY)
                {
                  voice[chan] = INS_PACK (chan);
                  a.instr_def = voice[chan];
                }
              }
              else
                a.instr_def = voice[chan];
            }

            switch (param >> 4)
            {
              case 0: // GLOBAL COMMANDS
                switch (param & 0x0F)
                {
                  case 1: // PATTERN BREAK
                    pat_break = line + 1;
                    break;

                  case 2: // SLIDE GLOBAL VOLUME UP
                  case 3: // SLIDE GLOBAL VOLUME DOWN
                  case 4: // SET GLOBAL VOLUME TO 0
                  case 5: // PERCUSSION MODE ON (N/A)
                  case 6: // PERCUSSION MODE OFF (N/A)
                  default:
                    break;
                }
                break;

              case 1: // MANUAL SLIDE UP
              case 2: // MANUAL SLIDE DOWN
              case 5: // SET PERCUSSION INSTRUMENT (N/A)
              case 6: // SET FEEDBACK
                break;

              case 10:  // SET CARRIER VOLUME
                if (a.instr_def == INS_EMPTY)
                {
                  if (!accurate_conv)
                    a.instr_def = voice[chan];
                  else if (voice[chan] == INS_EMPTY)
                    a.instr_def = INS_PACK (chan);
                }
                break;

              case 11:  // SET MODULATOR VOLUME
                if (a.instr_def == INS_EMPTY)
                {
                  if (!accurate_conv)
                    a.instr_def = voice[chan];
                  else if (voice[chan] == INS_EMPTY)
                    a.instr_def = INS_PACK (chan);
                }
                break;

              case 12:  // SET INSTRUMENT VOLUME
                if (a.instr_def == INS_EMPTY)
                {
                  if (!accurate_conv)
                    a.instr_def = voice[chan];
                  else if (voice[chan] == INS_EMPTY)
                    a.instr_def = INS_PACK (chan);
                }
                break;

              case 13:  // POSITION JUMP
              case 15:  // SET SPEED
              default: break;
            }
          }

          if (!bit_test (processed, pat)) (*pat_data)[chan][line] = a;
          event++;
        }
      bit_set (processed, pat);
    }
    if (pat >= count) break;
  }
}
