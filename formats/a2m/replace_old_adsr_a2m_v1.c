// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In: 'num_patterns' = 1..PATTERNS_MAX
static void replace_old_adsr_a2m_v1 (tFIXED_SONGDATA *song, uint8_t num_patterns,
                                     bool (*adsr_carrier)[CHANNELS_MAX_A2M_V1])
{
  uint8_t processed[(PATTERNS_MAX+7)/8];

  memset (processed, 0, sizeof (processed));
  memset (adsr_carrier, false, sizeof (*adsr_carrier));

  for (unsigned pos = 0; pos < PATTERN_ORDER_LEN; pos++)
    if (song->pattern_order[pos] < PATTERN_ORDER_JUMP)
    {
      unsigned idx = song->pattern_order[pos];
      tRAW_PATTERN_DATA *pat_data;

      if (idx >= num_patterns) break;

      pat_data = get_pattern_data (song, idx);

      if (pat_data != NULL)
      {
        unsigned break_to_line = PATTERN_LEN_A2M_V1; // no break

        for (unsigned line = 0; line < PATTERN_LEN_A2M_V1; line++)
          for (unsigned chan = 0; chan < CHANNELS_MAX_A2M_V1; chan++)
          {
            tCHUNK chunk, chunk2;

            chunk = (*pat_data)[chan][line];
            chunk2 = chunk;

            if (   (chunk.effect_def == ef_PositionJump)
                || (chunk.effect_def == ef_PatternBreak))
              break_to_line = line;

            if ((chunk.effect_def == 0xFF) || (chunk.effect_def == ef_Extended))
            {
              if (chunk.effect_def == 0xFF)
              {
                chunk2.effect_def = 0;
                chunk2.effect = 0;

                if (line <= break_to_line)
                  switch (chunk.effect)
                  {
                    case 0: (*adsr_carrier)[chan] = true; break;
                    case 1: (*adsr_carrier)[chan] = false; break;
                    default: break;
                  }
              }

              if (chunk.effect_def == ef_Extended)
                switch (chunk.effect >> 4)
                {
                  case ef_ex_SetAttckRateM:
                  case ef_ex_SetAttckRateC:
                    chunk2.effect = (((*adsr_carrier)[chan] ? ef_ex_SetAttckRateC : ef_ex_SetAttckRateM) << 4) + (chunk.effect & 0x0F);
                    break;

                  case ef_ex_SetDecayRateM:
                  case ef_ex_SetDecayRateC:
                    chunk2.effect = (((*adsr_carrier)[chan] ? ef_ex_SetDecayRateC : ef_ex_SetDecayRateM) << 4) + (chunk.effect & 0x0F);
                    break;

                  case ef_ex_SetSustnLevelM:
                  case ef_ex_SetSustnLevelC:
                    chunk2.effect = (((*adsr_carrier)[chan] ? ef_ex_SetSustnLevelC : ef_ex_SetSustnLevelM) << 4) + (chunk.effect & 0x0F);
                    break;

                  case ef_ex_SetRelRateM:
                  case ef_ex_SetRelRateC:
                    chunk2.effect = (((*adsr_carrier)[chan] ? ef_ex_SetRelRateC : ef_ex_SetRelRateM) << 4) + (chunk.effect & 0x0F);
                    break;

                  default: break;
                }

              if (   !bit_test (processed, idx)
                  && ((chunk.effect_def != chunk2.effect_def) || (chunk.effect != chunk2.effect)))
                (*pat_data)[chan][line] = chunk2;
            }
          }
      }

      bit_set (processed, idx);
    }
}
