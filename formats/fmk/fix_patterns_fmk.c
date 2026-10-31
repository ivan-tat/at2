// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

typedef struct
{
  uint8_t
    ins     [CHANNELS_MAX_FMK],
    misc    [CHANNELS_MAX_FMK],
    arpg    [CHANNELS_MAX_FMK],
    forcevol[CHANNELS_MAX_FMK],
    volsld  [CHANNELS_MAX_FMK],
    xfvolsld[CHANNELS_MAX_FMK],
    slide   [CHANNELS_MAX_FMK];
  uint8_t first_ins_load[(CHANNELS_MAX_FMK+7)/8];
  bool speed_table_fixed;
  struct
  {
    uint8_t
      effect_def,
      effect,
      effect_def2,
      effect2;
  } prev[CHANNELS_MAX_FMK];
} commands_cache_fmk_t;

static void fix_pattern_fmk (tRAW_PATTERN_DATA *pat_data, commands_cache_fmk_t *cache)
{
  unsigned pat_break;

  memset (cache->prev, 0, sizeof (cache->prev));

  if (!cache->speed_table_fixed)
    for (unsigned line = 0; line < PATTERN_LEN_FMK; line++)
    {
      for (unsigned chan = 0; chan < CHANNELS_MAX_FMK; chan++)
      {
        tCHUNK *a = &(*pat_data)[chan][line];

        if (a->effect_def == 0)
        {
          a->effect_def = ef_SetCustomSpeedTab;
          a->effect     = 0xFA;
          cache->speed_table_fixed = true;
          break;
        }
        else if (a->effect_def2 == 0)
        {
          a->effect_def2 = ef_SetCustomSpeedTab;
          a->effect2     = 0xFA;
          cache->speed_table_fixed = true;
          break;
        }
      }
      if (cache->speed_table_fixed) break;
    }

  pat_break = PATTERN_LEN_FMK;
  for (unsigned line = 0; line < PATTERN_LEN_FMK; line++)
    for (unsigned chan = 0; chan < CHANNELS_MAX_FMK; chan++)
    {
      tCHUNK *a = &(*pat_data)[chan][line];

      if (a->effect_def == temp_ef_rep)
        a->effect_def = cache->prev[chan].effect_def;

      if (a->effect_def == temp_ef_XFVSlide)
      {
        a->effect_def = ef_Extended2;
        if (cache->xfvolsld[chan] != 0)
          a->effect = (ef_ex2_VolSlideUpXF << 4) + (cache->volsld[chan] >> 4);
        else
          a->effect = (ef_ex2_VolSlideDnXF << 4) + (cache->volsld[chan] & 0x0F);
      }

      if (   (a->effect_def == ef_PositionJump)
          || (a->effect_def == ef_PatternBreak))
        pat_break = line;

      if (   (line <= pat_break)
          && (a->instr_def   != INS_EMPTY)
          && (a->instr_def   != cache->ins[chan])
          && (a->effect_def2 != ef_ForceInsVolume))
        cache->forcevol[chan] = 0;

      if ((   (a->effect_def == ef_Extended)
           && (a->effect     == (ef_ex_ExtendedCmd2 << 4) + ef_ex_cmd2_ResetVol))
           || (   (a->effect_def2 == ef_Extended)
               && (a->effect2     == (ef_ex_ExtendedCmd2 << 4) + ef_ex_cmd2_ResetVol)))
        cache->forcevol[chan] = 0;

      if (line <= pat_break)
      {
        if (a->effect_def2 == ef_ForceInsVolume)
          cache->forcevol[chan] = 1;

        if (a->instr_def != INS_EMPTY)
          cache->ins[chan] = a->instr_def;
      }

      if (line <= pat_break)
      {
        if (a->effect >> 4 != 0)
          cache->misc[chan] = (a->effect & 0xF0) + (cache->misc[chan] & 0x0F);
        else if (   (a->effect_def == ef_Vibrato)
                 || (a->effect_def == ef_Tremolo))
          a->effect = (cache->misc[chan] & 0xF0) + (a->effect & 0x0F);

        if ((a->effect & 0x0F) != 0)
          cache->misc[chan] = (cache->misc[chan] & 0xF0) + (a->effect & 0x0F);
        else if (   (a->effect_def == ef_Vibrato)
                 || (a->effect_def == ef_Tremolo))
          a->effect = (a->effect & 0xF0) + (cache->misc[chan] & 0x0F);

        if (a->effect_def == ef_RetrigNote)
        {
          if (a->effect != 0)
            cache->misc[chan] = a->effect;
          else
            a->effect = cache->misc[chan];
        }

        if (a->effect_def == temp_ef_Arpeggio)
        {
          if (a->effect != 0)
            cache->arpg[chan] = a->effect;
          else
            a->effect = cache->arpg[chan];
        }

        if (   (a->effect_def == ef_FSlideDown)
            || (a->effect_def == ef_FSlideDownFine)
            || (a->effect_def == ef_FSlideUp)
            || (a->effect_def == ef_FSlideUpFine)
            || (a->effect_def == ef_TonePortamento))
        {
          if (a->effect != 0)
            cache->slide[chan] = a->effect;
          else
            a->effect = cache->slide[chan];
        }

        if (   (a->effect_def == ef_Extended2)
            && (   (a->effect >> 4 == ef_ex2_FreqSlideDnXF)
                || (a->effect >> 4 == ef_ex2_FreqSlideUpXF)))
        {
          if ((a->effect & 0x0F) != 0)
            cache->slide[chan] = a->effect & 0x0F;
          else
            a->effect = (a->effect & 0xF0) + (cache->slide[chan] & 0x0F);
        }

        if (   (a->effect_def == ef_TPortamVolSlide)
            || (a->effect_def == ef_VibratoVolSlide)
            || (a->effect_def == ef_VolSlide)
            || (a->effect_def == ef_VolSlideFine))
        {
          if (a->effect != 0)
            cache->volsld[chan] = a->effect;
          else
            a->effect = cache->volsld[chan];
        }

        if (   (a->effect_def == ef_Extended2)
            && (   (a->effect >> 4 == ef_ex2_VolSlideDnXF)
                || (a->effect >> 4 == ef_ex2_VolSlideUpXF))
            && ((a->effect & 0x0F) != 0))
          switch (a->effect >> 4)
          {
            case ef_ex2_VolSlideDnXF:
              cache->volsld[chan] = a->effect & 0x0F;
              cache->xfvolsld[chan] = 0;
              break;

            case ef_ex2_VolSlideUpXF:
              cache->volsld[chan] = a->effect & 0xF0;
              cache->xfvolsld[chan] = 1;
              break;

            default: break;
          }
      }

      if (   (   (cache->prev[chan].effect_def == ef_Vibrato)
              || (cache->prev[chan].effect_def == ef_VibratoVolSlide))
          && (a->effect_def != ef_Vibrato)
          && (a->effect_def != ef_VibratoVolSlide))
      {
        if ((a->effect_def == 0) && (a->effect == 0))
        {
          unsigned effect_def, effect;

          effect_def = ef_Extended;
          effect     = (ef_ex_ExtendedCmd2 << 4) + ef_ex_cmd2_VibrOff;
          if (   (a->effect_def2 != effect_def)
              || (a->effect2     != effect))
          {
            a->effect_def = effect_def;
            a->effect     = effect;
          }
        }
        else if ((a->effect_def2 == 0) && (a->effect2 == 0))
        {
          unsigned effect_def2, effect2;

          effect_def2 = ef_Extended;
          effect2     = (ef_ex_ExtendedCmd2 << 4) + ef_ex_cmd2_VibrOff;
          if (   (a->effect_def != effect_def2)
              || (a->effect     != effect2))
          {
            a->effect_def2 = effect_def2;
            a->effect2     = effect2;
          }
        }
      }

      if (   (a->instr_def != INS_EMPTY)
          && (a->effect_def2 == 0)
          && (a->effect2 == 0)
          && (   bit_test (cache->first_ins_load, chan)
              || ((cache->forcevol[chan] != 0) && (line <= pat_break))))
      {
        if (   (a->effect_def != ef_SetModulatorVol)
            && (a->effect_def != ef_SetCarrierVol))
        {
          a->effect_def2 = ef_Extended;
          a->effect2     = (ef_ex_ExtendedCmd2 << 4) + ef_ex_cmd2_ResetVol;
        }
        else
        {
          a->effect_def2 = a->effect_def;
          a->effect2     = a->effect;
          a->effect_def  = ef_Extended;
          a->effect      = (ef_ex_ExtendedCmd2 << 4) + ef_ex_cmd2_ResetVol;
        }
        cache->forcevol[chan] = 0;
        bit_clear (cache->first_ins_load, chan);
      }

      cache->prev[chan].effect_def  = a->effect_def;
      cache->prev[chan].effect      = a->effect;
      cache->prev[chan].effect_def2 = a->effect_def2;
      cache->prev[chan].effect2     = a->effect2;

      if (is_4op_chan (chan + 1) && bit_test (_4op_tracks_hi, chan + 1))
      {
        tCHUNK *b = &(*pat_data)[chan + 1][line];

        if (   (a->instr_def == INS_EMPTY)
            && (b->instr_def != INS_EMPTY))
          a->instr_def = cache->ins[chan] != INS_EMPTY ? cache->ins[chan]
                                                       : b->instr_def;
      }

      if (a->effect_def == temp_ef_Arpeggio)
        a->effect_def = ef_Arpeggio;

      if (   (   (a->effect_def == ef_SetModulatorVol)
              || (a->effect_def == ef_SetCarrierVol))
          && (a->effect_def2 == ef_ForceInsVolume))
      {
        uint8_t effect_def, effect;

        effect_def     = a->effect_def;
        effect         = a->effect;
        a->effect_def  = a->effect_def2;
        a->effect      = a->effect2;
        a->effect_def2 = effect_def;
        a->effect2     = effect;
      }
    }
}

static void fix_patterns_fmk (tFIXED_SONGDATA *song, unsigned count)
{
  uint8_t processed[(PATTERNS_MAX+7)/8];
  uint8_t speed_table_fixed[(PATTERNS_MAX+7)/8];
  commands_cache_fmk_t cache;

  memset (processed, 0, sizeof (processed));
  memset (speed_table_fixed, 0, sizeof (speed_table_fixed));
  memset (cache.ins, INS_EMPTY, sizeof (cache.ins));
  memset (cache.first_ins_load, 0xFF, sizeof (cache.first_ins_load));
  memset (cache.xfvolsld, 0, sizeof (cache.xfvolsld));
  memset (cache.volsld, 0, sizeof (cache.volsld));
  memset (cache.slide, 0, sizeof (cache.slide));
  memset (cache.misc, 0, sizeof (cache.misc));
  memset (cache.arpg, 0, sizeof (cache.arpg));
  memset (cache.forcevol, 0, sizeof (cache.forcevol));

  for (unsigned pos = 0; pos < PATTERN_ORDER_LEN; pos++)
  {
    unsigned pat = song->pattern_order[pos];

    if (pat < count)
    {
      if (touch_pattern (song, pat))
      {
        tRAW_PATTERN_DATA *pat_data = get_pattern_data (song, pat);

        cache.speed_table_fixed = bit_test (speed_table_fixed, pat);
        fix_pattern_fmk (pat_data, &cache);
        bit_assign (speed_table_fixed, pat, cache.speed_table_fixed);
        bit_set (processed, pat);
      }
    }
    else
      break;
  }

  for (unsigned pat = 0; pat < count; pat++)
    if (!bit_test (processed, pat) && touch_pattern (song, pat))
    {
      tRAW_PATTERN_DATA *pat_data = get_pattern_data (song, pat);

      cache.speed_table_fixed = bit_test (speed_table_fixed, pat);
      fix_pattern_fmk (pat_data, &cache);
    }
}
