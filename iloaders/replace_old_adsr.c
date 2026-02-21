// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void replace_old_adsr (uint8_t patterns)
{
  tFIXED_SONGDATA *song = &songdata;
  uint8_t order = 0;
  uint8_t patts_count = 0;
  uint8_t patts[255], patt;

  memset (adsr_carrier, false, sizeof (adsr_carrier));

  do
  {
    patt = song->pattern_order[order];

    if (patt >= 0x80) order++;
    else
    {
      uint8_t patt_break = BYTE_NULL;

      for (uint8_t i = 0; i < 0x40; i++)
        for (uint8_t j = 0; j < 9; j++)
        {
          tCHUNK chunk, chunk2;

          get_chunk (patt, i, j + 1, &chunk);
          chunk2 = chunk;

          if (   (chunk.effect_def == ef_PositionJump)
              || (chunk.effect_def == ef_PatternBreak))
            patt_break = i;

          if ((chunk.effect_def == 0xFF) || (chunk.effect_def == ef_Extended))
          {
            if (chunk.effect_def == 0xFF)
            {
              chunk2.effect_def = 0;
              chunk2.effect = 0;

              if (i <= patt_break)
                switch (chunk.effect)
                {
                  case 0: adsr_carrier[j] = true; break;
                  case 1: adsr_carrier[j] = false; break;
                  default: break;
                }
            }

            if (chunk.effect_def == ef_Extended)
              switch (chunk.effect >> 4)
              {
                case ef_ex_SetAttckRateM:
                case ef_ex_SetAttckRateC:
                  chunk2.effect = ((adsr_carrier[j] ? ef_ex_SetAttckRateC : ef_ex_SetAttckRateM) << 4) + (chunk.effect & 0x0F);
                  break;

                case ef_ex_SetDecayRateM:
                case ef_ex_SetDecayRateC:
                  chunk2.effect = ((adsr_carrier[j] ? ef_ex_SetDecayRateC : ef_ex_SetDecayRateM) << 4) + (chunk.effect & 0x0F);
                  break;

                case ef_ex_SetSustnLevelM:
                case ef_ex_SetSustnLevelC:
                  chunk2.effect = ((adsr_carrier[j] ? ef_ex_SetSustnLevelC : ef_ex_SetSustnLevelM) << 4) + (chunk.effect & 0x0F);
                  break;

                case ef_ex_SetRelRateM:
                case ef_ex_SetRelRateC:
                  chunk2.effect = ((adsr_carrier[j] ? ef_ex_SetRelRateC : ef_ex_SetRelRateM) << 4) + (chunk.effect & 0x0F);
                  break;

                default: break;
              }

            if ((patts_count == 0) || (memchr (patts, patt, patts_count) == NULL))
              if ((chunk.effect_def != chunk2.effect_def) || (chunk.effect != chunk2.effect))
                put_chunk (patt, i, j + 1, &chunk2);
          }
        }

      order++;
      if (patts_count < sizeof (patts)) patts[patts_count++] = patt;
    }
  } while ((patt < patterns) || (order >= 0x80));
}
