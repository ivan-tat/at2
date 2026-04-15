// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Scream Tracker 3.x song loader (*.s3m)

#define S3M_OCTAVES 8
#define S3M_INSTRUMENTS_MAX 99
#define S3M_CHANNELS_MAX 32
#define S3M_PATTERN_LEN 64
#define S3M_PATTERNS_MAX 100
#define S3M_ORDER_LEN 255

#define S3M_SONG_ID       (MK_UINT32 ('S','C','R','M'))
#define S3M_ADLIB_INS_ID  (MK_UINT32 ('S','C','R','I'))
#define S3M_SAMPLE_INS_ID (MK_UINT32 ('S','C','R','S'))

// channels set
#define S3M_CHAN_MUTED  0x80
#define S3M_CHAN_UNUSED 0xFF

#pragma pack(push, 1)
typedef struct  // all values are little-endian
{
  char title[28];   // ASCIIZ
  uint8_t byte1a;   // =0x1A
  uint8_t ftype;    // File type: 16=ST 3.x module
  uint8_t unused1[2];
  uint16_t ordnum;  // Number of orders in file (should be even)
  uint16_t insnum;  // Number of instruments in file
  uint16_t patnum;  // Number of patterns in file
  uint16_t flags;   // * bit 0: ST 2.x vibrato (not supported in ST 3.01)
                    // * bit 1: ST 2.x tempo (not supported in ST 3.01)
                    // * bit 2: Amiga slides (not supported in ST 3.01)
                    // * bit 3: Zero volume optimizations
                    //          Automatically turn off looping notes whose volume is zero for >2 note rows.
                    // * bit 4: Amiga limits
                    //          Disallow any notes that go beyond the Amiga hardware limits (like Amiga does).
                    //          This means that sliding up stops at B#5 etc.
                    //          Also affects some minor Amiga compatibility issues.
                    // * bit 5: Enable SB filter/sfx (not supported in ST 3.01)
                    // * bit 6: ST 3.00 volume slides
                    //          Normally volume slide is not performed on first frame of each row (this is according to Amiga playing).
                    //          If this is set, volume slide is performed ALSO on the first row.
                    //          This is set by default if song was saved with ST 3.0.
                    // * bit 7: Special custom data in file
  uint16_t cwt_v;   // Created with tracker / version: bits 15..12=tracker (1=ST), bits 11..0=version
                    // * 0x1300=ST 3.00 (volume slides on EVERY frame)
                    // * 0x1301=ST 3.01
                    // * 0x1303=ST 3.03
                    // * 0x1320=ST 3.20
  uint16_t ffi;     // File format information:
                    // * 1=signed samples (VERY OLD)
                    // * 2=unsigned samples
  uint32_t id;      // ="SCRM"
  uint8_t g_v;      // Global volume
  uint8_t i_s;      // Initial speed (command `A')
  uint8_t i_t;      // Initial tempo (command `T')
  uint8_t m_v;      // bit 7: 0=mono, 1=stereo, bits 6..0: master volume
  uint8_t u_c;      // Ultra click removal
  uint8_t d_p;      // =252 if default channel pan positions are present in the end of the header.
                    // Do not try to load channel pan settings otherwise.
  uint8_t unused2[8];
  uint16_t special;
  uint8_t chan_set[S3M_CHANNELS_MAX];
} s3m_header_t;

typedef struct  // all values are little-endian
{
  uint8_t type;       // AdLib: 2=melody, 3=bass drum, 4=snare drum, 5=tom-tom, 6=top cymbal, 7=hi-hat
  char dosname[12];   // 8.3, may not be NULL-terminated
  uint8_t unused1[3];
  uint8_t fmdata[12]; // D00..D0B contains the adlib instrument specs packed like this:
                      // Modulator:                                             Carrier:
                      // D00=[?vol.vib]*128 + [?pitch vib]*64 + [?sustain]*32   =D01
                      //     + [?scale env.]*16 + [freq.muliplier]
                      // D02=[levelscale&1]*128 + [l.s.&2]*64 + [63-volume]     =D03
                      // D04=[attack]*16 + [decay]                              =D05
                      // D06=[15-sustain]*16 + [release]                        =D07
                      // D08=[wave select]                                      =D09
                      // D0A=[modulation feedback]*2+[?additive synthesis]
                      // D0B=unused
  uint8_t vol;        // Default volume (0..64)
  uint8_t unused2[3];
  uint16_t freq;      // Middle C frequency (Hertz). ST 3.x uses only lower 16 bits.
                      // Actually this is a modifier since there is no clear frequency for AdLib instruments.
                      // It scales the note frequency sent to AdLib.
  uint16_t freq_hi;   // unused
  uint8_t unused3[12];
  char title[28];     // ASCIIZ
  uint32_t id;        // ="SCRI" or "SCRS"
} s3m_adlib_ins_t;
#pragma pack(pop)

// patterns order
#define S3M_ORDER_SKIP  0xFE
#define S3M_ORDER_STOP  0xFF

typedef struct  // all values are little-endian
{
  uint8_t order[S3M_ORDER_LEN];               // patterns order
  uint16_t paraptr_ins[S3M_INSTRUMENTS_MAX];  // para pointers (x16) to instruments
  uint16_t paraptr_pat[S3M_PATTERNS_MAX];     // para pointers (x16) to patterns
  uint8_t def_vol[S3M_INSTRUMENTS_MAX];       // default instruments volume
  int8_t c4factor[S3M_INSTRUMENTS_MAX];       // instruments transposition
} s3m_loader_data_t;

// event
#define S3M_NOTE_OFF    0xFE
#define S3M_NOTE_EMPTY  0xFF
#define S3M_INS_EMPTY   0
#define S3M_VOL_EMPTY   0xFF
#define S3M_CMD_EMPTY   0

typedef struct
{
  uint8_t
    chan, // 0..S3M_CHANNELS_MAX-1
    note,
    ins,
    vol,
    cmd,
    info;
} s3m_event_t;

typedef struct
{
  struct
  {
    uint8_t
      effect_def,
      effect,
      effect_def2,
      effect2;
  } prev[AT_MELODIC_CHANNELS_MAX];
  uint8_t
    ins    [AT_MELODIC_CHANNELS_MAX],
    note   [AT_MELODIC_CHANNELS_MAX],
    volsld [AT_MELODIC_CHANNELS_MAX],
    slide  [AT_MELODIC_CHANNELS_MAX],
    misc   [AT_MELODIC_CHANNELS_MAX],
    arpg   [AT_MELODIC_CHANNELS_MAX],
    patloop[AT_MELODIC_CHANNELS_MAX];
} s3m_commands_cache_t;

#define S3M_VIBRATO_COEF(octave) 0.55f + 0.20f * octave
static const float s3m_vibrato_lut[S3M_OCTAVES] =
{
  S3M_VIBRATO_COEF(0),
  S3M_VIBRATO_COEF(1),
  S3M_VIBRATO_COEF(2),
  S3M_VIBRATO_COEF(3),
  S3M_VIBRATO_COEF(4),
  S3M_VIBRATO_COEF(5),
  S3M_VIBRATO_COEF(6),
  S3M_VIBRATO_COEF(7)
};

static void fix_s3m_pattern (const int8_t *c4factor, s3m_commands_cache_t *cache, uint8_t patt)
{
  uint8_t patt_break = UINT8_NULL;

  memset (cache->prev, 0, sizeof (cache->prev));
  memset (cache->patloop, UINT8_NULL, sizeof (cache->patloop));

  for (uint8_t line = 0; line < S3M_PATTERN_LEN; line++)
    for (uint8_t chan = 0; chan < AT_MELODIC_CHANNELS_MAX; chan++)
    {
      tCHUNK a;

      get_chunk (patt, line, chan + 1, &a);

      if (   (a.effect_def == ef_PositionJump)
          || (a.effect_def == ef_PatternBreak))
        patt_break = line;

      if (line <= patt_break)
      {
        if (a.instr_def != 0)
          cache->ins[chan] = a.instr_def;

        if ((a.note >= 1) && (a.note <= 12*8+1))
          cache->note[chan] = a.note;
      }

      if (   (a.instr_def != 0)
          || ((cache->ins[chan] != 0) && (a.note >= 1) && (a.note <= 12*8+1)))
      {
        uint8_t ins = (a.instr_def != 0 ? a.instr_def : cache->ins[chan]) - 1;

        if (c4factor[ins] != 0)
        {
          a.note = c4factor[ins] != -127
                 ? min (max (a.note + c4factor[ins], 12*8+1), 1)
                 : 1;
          put_chunk (patt, line, chan + 1, &a);
        }
      }

      if (   (a.effect_def == ef_Extended)
          && ((a.effect >> 4) == ef_ex_PatternLoop)
          && ((a.effect & 0x0F) != 0)
          && (line != 0))
      {
        if (   (cache->patloop[chan] != 0)
            && (cache->patloop[chan] != UINT8_NULL))
        {
          if (   (cache->prev[chan].effect_def == 0)
              && (cache->prev[chan].effect == 0))
          {
            tCHUNK b;

            get_chunk (patt, line - 1, chan + 1, &b);

            b.effect_def = ef_Extended;
            b.effect     = ef_ex_PatternLoop << 4;
            if (   (b.effect_def != b.effect_def2)
                || (b.effect     != b.effect2))
            {
              put_chunk (patt, line - 1, chan + 1, &b);
              cache->prev[chan].effect_def = a.effect_def;
              cache->prev[chan].effect     = a.effect;
            }
          }
          else if (   (cache->prev[chan].effect_def2 == 0)
                   && (cache->prev[chan].effect2 == 0))
          {
            tCHUNK b;

            get_chunk (patt, line - 1, chan + 1, &b);

            b.effect_def2 = ef_Extended;
            b.effect2     = ef_ex_PatternLoop << 4;
            if (   (b.effect_def2 != b.effect_def)
                || (b.effect2     != b.effect))
            {
              put_chunk (patt, line - 1, chan + 1, &b);
              cache->prev[chan].effect_def2 = a.effect_def2;
              cache->prev[chan].effect2     = a.effect2;
            }
          }
        }
        else if (cache->patloop[chan] != 0)
        {
          tCHUNK b;

          get_chunk (patt, 0, chan + 1, &b);

          if ((b.effect_def == 0) && (b.effect == 0))
          {
            b.effect_def = ef_Extended;
            b.effect     = ef_ex_PatternLoop << 4;
            if (   (b.effect_def != b.effect_def2)
                || (b.effect     != b.effect2))
              put_chunk (patt, 0, chan + 1, &b);
          }
          else if ((b.effect_def2 == 0) && (b.effect2 == 0))
          {
            b.effect_def2 = ef_Extended;
            b.effect2     = ef_ex_PatternLoop << 4;
            if (   (b.effect_def2 != b.effect_def)
                || (b.effect2     != b.effect))
              put_chunk (patt, 0, chan + 1, &b);
          }
        }
      }

      if (line <= patt_break)
      {
        if ((a.effect >> 4) != 0)
          cache->misc[chan] = (a.effect & 0xF0) + (cache->misc[chan] & 0x0F);
        else if (   (a.effect_def == ef_Vibrato)
                 || (a.effect_def == ef_ExtraFineVibrato)
                 || (a.effect_def == ef_Tremolo)
                 || (a.effect_def == ef_Tremor)
                 || (a.effect_def == ef_MultiRetrigNote))
        {
          a.effect = (cache->misc[chan] & 0xF0) + (a.effect & 0x0F);
          put_chunk (patt, line, chan + 1, &a);
        }

        if ((a.effect & 0x0F) != 0)
          cache->misc[chan] = (cache->misc[chan] & 0xF0) + (a.effect & 0x0F);
        else if (   (a.effect_def == ef_Vibrato)
                 || (a.effect_def == ef_ExtraFineVibrato)
                 || (a.effect_def == ef_Tremolo)
                 || (a.effect_def == ef_Tremor)
                 || (a.effect_def == ef_MultiRetrigNote))
        {
          a.effect = (a.effect & 0xF0) + (cache->misc[chan] & 0x0F);
          put_chunk (patt, line, chan + 1, &a);
        }

        if (a.effect_def == temp_ef_Arpeggio)
        {
          if (a.effect != 0)
            cache->arpg[chan] = a.effect;
          else
          {
            a.effect = cache->arpg[chan];
            put_chunk (patt, line, chan + 1, &a);
          }
        }

        if (   (a.effect_def == ef_FSlideDown)
            || (a.effect_def == ef_FSlideDownFine)
            || (a.effect_def == ef_FSlideUp)
            || (a.effect_def == ef_FSlideUpFine)
            || (a.effect_def == ef_TonePortamento))
        {
          if (a.effect != 0)
            cache->slide[chan] = a.effect;
          else
          {
            a.effect = cache->slide[chan];
            put_chunk (patt, line, chan + 1, &a);
          }
        }

        // experimental method to fix up frequency slide
        if (   (a.effect_def == ef_FSlideDown)
            || (a.effect_def == ef_FSlideDownFine)
            || (a.effect_def == ef_FSlideUp)
            || (a.effect_def == ef_FSlideUpFine)
            || (a.effect_def == ef_Vibrato)
            || (a.effect_def == ef_ExtraFineVibrato)
            || (a.effect_def == ef_TonePortamento))
        {
          if (cache->note[chan] != 0)
          {
            bool vibrato =  (a.effect_def == ef_Vibrato)
                         || (a.effect_def == ef_ExtraFineVibrato);
            uint8_t fx;
            uint8_t octave;

            if (vibrato)
            {
              fx = a.effect & 0xF0;
              a.effect &= 0x0F;
            }

            octave = (cache->note[chan] - 1) / 12;
            if (octave < S3M_OCTAVES)
              a.effect = max (a.effect * s3m_vibrato_lut[octave], 255);

            if (vibrato)
              a.effect = max (a.effect, 0x0F) + fx;

            put_chunk (patt, line, chan + 1, &a);
          }
        }

        if (   (a.effect_def == ef_Extended2)
            && (   ((a.effect >> 4) == ef_ex2_FreqSlideDnXF)
                || ((a.effect >> 4) == ef_ex2_FreqSlideUpXF)))
        {
          if ((a.effect & 0x0F) != 0)
            cache->slide[chan] = a.effect & 0x0F;
          else
          {
            a.effect = (a.effect & 0xF0) + (cache->slide[chan] & 0x0F);
            put_chunk (patt, line, chan + 1, &a);
          }
        }

        if ((   (a.effect_def == ef_TPortamVolSlide)
             || (a.effect_def == ef_VibratoVolSlide)
             || (a.effect_def == ef_VolSlide)
             || (a.effect_def == ef_VolSlideFine))
            && (line <= patt_break))
        {
          if (a.effect != 0)
            cache->volsld[chan] = a.effect;
          else
          {
            a.effect = cache->volsld[chan];
            put_chunk (patt, line, chan + 1, &a);
          }
        }

        if (   (a.effect_def == ef_Extended2)
            && (   ((a.effect >> 4) == ef_ex2_VolSlideDnXF)
                || ((a.effect >> 4) == ef_ex2_VolSlideUpXF)))
        {
          if ((a.effect & 0x0F) != 0)
            switch (a.effect >> 4)
            {
              case ef_ex2_VolSlideDnXF: cache->volsld[chan] = a.effect & 0x0F; break;
              case ef_ex2_VolSlideUpXF: cache->volsld[chan] = (a.effect & 0x0F) << 4; break;
              default: break;
            }
          else
          {
            switch (a.effect >> 4)
            {
              case ef_ex2_VolSlideDnXF: a.effect = (a.effect & 0xF0) + (cache->volsld[chan] & 0x0F); break;
              case ef_ex2_VolSlideUpXF: a.effect = (cache->volsld[chan] & 0xF0) + (a.effect & 0x0F); break;
              default: break;
            }
            put_chunk (patt, line, chan + 1, &a);
          }
        }
      }

      if ((   (cache->prev[chan].effect_def == ef_Vibrato)
           || (cache->prev[chan].effect_def == ef_ExtraFineVibrato)
           || (cache->prev[chan].effect_def == ef_VibratoVolSlide))
          && (a.effect_def != ef_Vibrato)
          && (a.effect_def != ef_ExtraFineVibrato)
          && (a.effect_def != ef_VibratoVolSlide))
      {
        if ((a.effect_def == 0) && (a.effect == 0))
        {
          tCHUNK b = a;

          b.effect_def = ef_Extended;
          b.effect     = (ef_ex_ExtendedCmd2 << 4) + ef_ex_cmd2_VibrOff;
          if (   (b.effect_def != b.effect_def2)
              || (b.effect     != b.effect2))
          {
            put_chunk (patt, line, chan + 1, &b);
            a = b;
          }
        }
        else if ((a.effect_def2 == 0) && (a.effect2 == 0))
        {
          tCHUNK b = a;

          b.effect_def2 = ef_Extended;
          b.effect2     = (ef_ex_ExtendedCmd2 << 4) + ef_ex_cmd2_VibrOff;
          if (   (b.effect_def2 != b.effect_def)
              || (b.effect2     != b.effect))
          {
            put_chunk (patt, line, chan + 1, &b);
            a = b;
          }
        }
      }

      if (   (a.effect_def == ef_Extended)
          && ((a.effect >> 4) == ef_ex_PatternLoop))
        cache->patloop[chan] = a.effect & 0x0F;

      cache->prev[chan].effect_def  = a.effect_def;
      cache->prev[chan].effect      = a.effect;
      cache->prev[chan].effect_def2 = a.effect_def2;
      cache->prev[chan].effect2     = a.effect2;

      if (a.effect_def == temp_ef_Arpeggio)
      {
        tCHUNK b = a;

        b.effect_def = ef_Arpeggio;
        put_chunk (patt, line, chan + 1, &b);
      }
    }
}

static void fix_s3m_commands (const int8_t *c4factor, uint8_t patterns)
{
  tFIXED_SONGDATA *song = &songdata;
  s3m_commands_cache_t cache;
  bool patts[AT_PATTERNS_MAX];
  uint8_t order = 0;
  uint8_t patt = UINT8_NULL;

  memset (cache.ins, 0, sizeof (cache.ins));
  memset (cache.note, 0, sizeof (cache.note));
  memset (cache.volsld, 0, sizeof (cache.volsld));
  memset (cache.slide, 0, sizeof (cache.slide));
  memset (cache.misc, 0, sizeof (cache.misc));
  memset (cache.arpg, 0, sizeof (cache.arpg));
  memset (patts, false, sizeof (patts));

  do
  {
    uint8_t x = song->pattern_order[order++];

    if (x < AT_PATTERNS_MAX)
    {
      patt = x;
      if (!patts[patt])
      {
        fix_s3m_pattern (c4factor, &cache, patt);
        patts[patt] = true;
      }
    }
  } while ((patt < patterns) && (order < AT_ORDER_LEN));

  for (patt = 0; patt < patterns; patt++)
    if (!patts[patt]) fix_s3m_pattern (c4factor, &cache, patt);
}

static uint8_t scale_s3m_slide (uint8_t slide)
{
  if (slide > 16)
    return 16.0f + slide / 8.0f;
  else
    return slide * (2.0f - (slide / 16.0f));
}

void import_s3m_event (uint8_t patt, uint8_t line, s3m_event_t *ev, uint8_t *def_vol)
{
  tCHUNK a;

  memset (&a, 0, sizeof (a));
  a.instr_def = ev->ins == S3M_INS_EMPTY ? AT_INS_EMPTY : ev->ins;

  switch (ev->note)
  {
    case S3M_NOTE_OFF:
      a.note = AT_NOTE_OFF;
      break;
    case S3M_NOTE_EMPTY:
      a.note = AT_NOTE_EMPTY;
      break;
    default:
      if ((ev->note & 0x0F) <= 11)
        a.note = 12 * ((ev->note >> 4) & 7) + (ev->note & 0x0F) + 1;
      break;
  }

  if (ev->vol != S3M_VOL_EMPTY)
  {
    a.effect_def2 = ef_SetInsVolume;
    a.effect2     = max (ev->vol, 63);
  }
  else
    if (   (ev->note != S3M_NOTE_OFF)
        && (ev->note != S3M_NOTE_EMPTY)
        && (ev->ins != 0) && (max (def_vol[ev->ins - 1], 63) != 63))
    {
      a.effect_def2 = ef_SetInsVolume;
      a.effect2     = max (def_vol[ev->ins - 1], 63);
    }

  switch (ev->cmd)
  {
    case S3M_CMD_EMPTY: // NONE
      a.effect = ev->info;
      break;

    case 1: // A: SET SPEED
      if (ev->info != 0)
      {
        a.effect_def = ef_SetSpeed;
        a.effect     = ev->info;
      }
      break;

    case 2: // B: JUMP TO ORDER
      if (ev->info < S3M_ORDER_LEN)
      {
        a.effect_def = ef_PositionJump;
        a.effect     = ev->info;
      }
      break;

    case 3: // C: PATTERN BREAK
      if (ev->info < S3M_PATTERN_LEN)
      {
        a.effect_def = ef_PatternBreak;
        a.effect     = (ev->info >> 4) * 10 + (ev->info & 0x0F);
      }
      break;

    // VOLUME SLIDE
    case 4: // D: VOLUME SLIDE DOWN
      switch (ev->info >> 4)
      {
        case 0: // NORMAL
          a.effect_def = ef_VolSlide;
          a.effect     = ev->info & 0x0F;
          break;

        case 15:  // FINE
          a.effect_def = ef_VolSlideFine;
          a.effect     = ev->info & 0x0F;
          break;

        default:  // VOLUME SLIDE UP
          switch (ev->info & 0x0F)
          {
            case 0: // NORMAL
              a.effect_def = ef_VolSlide;
              a.effect     = ev->info & 0xF0;
              break;

            case 15: // FINE
              a.effect_def = ef_VolSlideFine;
              a.effect     = ev->info & 0xF0;
              break;

            default: break;
          }
          break;
      }
      break;

    case 5: // E: SLIDE DOWN
      switch (ev->info >> 4)
      {
        case 14:  // EXTRA FINE
          a.effect_def = ef_Extended2;
          a.effect     = ef_ex2_FreqSlideDnXF << 4;
          if (ev->info != 0)
            a.effect += min ((ev->info & 0x0F) / 4, 1);
          break;

        case 15:  // FINE
          a.effect_def = ef_FSlideDownFine;
          a.effect     = ev->info & 0x0F;
          break;

        default:  // NORMAL
          a.effect_def = ef_FSlideDown;
          a.effect     = scale_s3m_slide (ev->info);
          break;
      }
      break;

    case 6: // F: SLIDE UP
      switch (ev->info >> 4)
      {
        case 14:  // EXTRA FINE
          a.effect_def = ef_Extended2;
          a.effect     = ef_ex2_FreqSlideUpXF << 4;
          if (ev->info != 0)
            a.effect += min ((ev->info & 0x0F) / 4, 1);
          break;

        case 15:  // FINE
          a.effect_def = ef_FSlideUpFine;
          a.effect     = ev->info & 0x0F;
          break;

        default:  // NORMAL
          a.effect_def = ef_FSlideUp;
          a.effect     = scale_s3m_slide (ev->info);
          break;
      }
      break;

    case 7: // G: TONE PORTAMENTO
      a.effect_def = ef_TonePortamento;
      a.effect     = scale_s3m_slide (ev->info);
      break;

    case 8: // H: VIBRATO
      a.effect_def = ef_Vibrato;
      a.effect     = ev->info;
      break;

    case 9: // I: TREMOR
      a.effect_def = ef_Tremor;
      a.effect     = ev->info;
      break;

    case 10:  // J: ARPEGGIO
      a.effect_def = temp_ef_Arpeggio;
      a.effect     = ev->info;
      break;

    case 11:  // K: VIBRATO + VOLUME SLIDE
      a.effect_def = ef_VibratoVolSlide;
      a.effect     = ev->info;
      break;

    case 12: // L: TONE PORTAMENTO + VOLUME SLIDE
      a.effect_def = ef_TPortamVolSlide;
      a.effect     = ev->info;
      break;

    case 13:  // M: n/a
    case 14:  // N: n/a
    case 15:  // O: SET SAMPLE OFFSET (unused)
    case 16:  // P: n/a
      a.effect = ev->info;
      break;

    case 17:  // Q: RETRIG NOTE + VOLUME SLIDE
      a.effect_def = ef_MultiRetrigNote;
      a.effect     = (ev->info << 4) + (ev->info >> 4);
      break;

    case 18:  // R: TREMOLO
      a.effect_def = ef_Tremolo;
      a.effect     = ev->info;
      break;

    case 19:  // S: SPECIAL COMMAND
      switch (ev->info >> 4)
      {
        case 11:  // PATTERN LOOP
          a.effect_def = ef_Extended;
          a.effect     = (ef_ex_PatternLoop << 4) + (ev->info & 0x0F);
          break;

        case 12:  // NOTE CUT
          a.effect_def = ef_Extended2;
          a.effect     = (ef_ex2_NoteCut << 4) + (ev->info & 0x0F);
          break;

        case 13:  // NOTE DELAY
          a.effect_def = ef_Extended2;
          a.effect     = (ef_ex2_NoteDelay << 4) + (ev->info & 0x0F);
          break;

        case 14:  // PATTERN DELAY
          a.effect_def = ef_Extended2;
          a.effect     = (ef_ex2_PatDelayRow << 4) + (ev->info & 0x0F);
          break;

        default: break;
      }
      break;

    case 20:  // T: TEMPO
      if (ev->info >= 32)
      {
        a.effect_def = ef_SetTempo;
        a.effect     = ev->info / 2.5f;
      }
      break;

    case 21:  // U: FINE VIBRATO
      a.effect_def = ef_ExtraFineVibrato;
      a.effect     = ev->info;
      break;

    case 22:  // V: SET GLOBAL VOLUME
      a.effect_def = ef_SetGlobalVolume;
      a.effect     = max (ev->info, 63);
      break;

    case 23:  // W: n/a
    case 24:  // X: n/a
    case 25:  // Y: n/a
    case 26:  // Z: n/a
      a.effect = ev->info;
      break;

    default: break;
  }

  if ((a.effect_def == 0) && (a.effect != 0))
    a.effect = 0;
  put_chunk (patt, line, ev->chan + 1, &a);
}

// experimental method to fix up note fine-tuning

static const float _s3m_factor[8] =
{
  1.0f / 8.0f,  // -3
  1.0f / 4.0f,  // -2
  1.0f / 2.0f,  // -1
  1.0f,         // 0
  2.0f,         // +1
  4.0f,         // +2
  8.0f,         // +3
  16.0f         // +4
};

static const uint16_t _s3m_freq[13] =
{
  33453 / 4,  // C-2
  35441 / 4,  // C#2
  37679 / 4,  // D-2
  39772 / 4,  // D#2
  42441 / 4,  // E-2
  44744 / 4,  // F-2
  47727 / 4,  // F#2
  50416 / 4,  // G-2
  53426 / 4,  // G#2
  56370 / 4,  // A-2
  59658 / 4,  // A#2
  63354 / 4,  // B-2
  33453 / 2   // C-3
};

static const uint16_t _s3m_fm_freq[13] =
{
  0x156, 0x16B, 0x181, 0x198, 0x1B0, 0x1CA,
  0x1E5, 0x202, 0x220, 0x241, 0x263, 0x287,
  0x2AE
};

static int8_t find_s3m_scale_factor (int32_t freq, int8_t *fine_tune)
{
  for (int8_t scaler = 0; scaler < 8; scaler++)
    for (int8_t i = 0; i < 12; i++)
    {
      float factor = _s3m_factor[scaler];
      int_least32_t a = freq - (int_least32_t)((double)_s3m_freq[i] * factor);
      int_least32_t b = (int_least32_t)((double)_s3m_freq[i + 1] * factor) - freq;

      if ((a >= 0) && (b >= 0))
      {
        if (a < b)
        {
          *fine_tune = (int8_t)((double)  (_s3m_fm_freq[i + 1] - _s3m_fm_freq[i])
                                        / (_s3m_freq[i + 1]    - _s3m_freq[i]) * a);
          return 12 * (scaler - 3) + i;
        }
        else
        {
          *fine_tune = (int8_t)((double)  (_s3m_fm_freq[i + 1] - _s3m_fm_freq[i])
                                        / (_s3m_freq[i + 1]    - _s3m_freq[i]) * (-b));
          return i != 11 ? 12 * (scaler - 3) + i + 1 : (12 * (scaler - 2));
        }
      }
    }

  *fine_tune = 0;
  return -127;
}

// In:
//   * `progress', `state' and `error' may be NULL.
//
// On success:
//   * Return value is 0.
//   * `state' and `error' are untouched.
// On error:
//   * Return value:
//     -1: open/read error.
//     -2: unknown file format.
//     -3: unsupported file format version.
//     -4: format is supported.
//   * `state' (if set):
//     0: song is untouched.
//     1: song is cleared.
//     2: song is partly loaded.
//   * `error' (if set) is set to error description.
int8_t s3m_file_loader (const String *_fname, progress_callback_t *progress, uint8_t *state, char **error)
{
  tFIXED_SONGDATA *song = &songdata;
  int8_t result = -1;
  uint8_t result_state = 0;
  char *result_error = NULL;
  FILE *f = NULL;
  long fsize;
  s3m_header_t header;
  s3m_loader_data_t *data = NULL;
  void *packed_data = NULL;
  size_t packed_size;
  uint8_t num_channels;
  char fname[255+1];

  DBG_ENTER ("s3m_file_loader");

  if (progress != NULL)
  {
    snprintf (progress->msg, sizeof (progress->msg), "Reading file...");
    progress->num_steps = 5;
    progress->value = 1;
    progress->update (progress, 0, -1);
  }

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "rb")) == NULL) goto _err_fopen;

  result = -2;

  // check file size
  if (fseek (f, 0, SEEK_END) != 0) goto _err_fread;
  if ((fsize = ftell (f)) < 0) goto _err_fread;
  if (fsize < (long)sizeof (header)) goto _err_format;
  if (fseek (f, 0, SEEK_SET) != 0) goto _err_fread;

  result = -1;

  if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;

  result = -2;

  if (   (header.id != S3M_SONG_ID)
      || (header.byte1a != 0x1A)
      || (header.ftype != 16)) goto _err_format;

  result = -4;

  header.ordnum = uint16_LE (header.ordnum);
  header.insnum = uint16_LE (header.insnum);
  header.patnum = uint16_LE (header.patnum);
/*
  header.flags = uint16_LE (header.flags);
  header.cwt_v = uint16_LE (header.cwt_v);
  header.ffi = uint16_LE (header.ffi);
  header.special = uint16_LE (header.special);
*/

  if (   (header.ordnum > S3M_ORDER_LEN)
      || (header.insnum > S3M_INSTRUMENTS_MAX)
      || (header.patnum > S3M_PATTERNS_MAX)) goto _err_format;

  // find playable channels
  for (num_channels = AT_MELODIC_CHANNELS_MAX; num_channels != 0; num_channels--)
    if (header.chan_set[num_channels - 1] != S3M_CHAN_UNUSED) break;
  if (num_channels == 0) goto _err_chan;

  // read variable header data
  if ((data = malloc (sizeof (*data))) == NULL) goto _err_malloc;
  if (fread (&data->order, header.ordnum * sizeof (data->order[0]), 1, f) == 0) goto _err_fread;
  if (fread (&data->paraptr_ins, header.insnum * sizeof (data->paraptr_ins[0]), 1, f) == 0) goto _err_fread;
  if (fread (&data->paraptr_pat, header.patnum * sizeof (data->paraptr_pat[0]), 1, f) == 0) goto _err_fread;

  for (uint16_t i = 0; i < header.insnum; i++) data->paraptr_ins[i] = uint16_LE (data->paraptr_ins[i]);
  for (uint16_t i = 0; i < header.patnum; i++) data->paraptr_pat[i] = uint16_LE (data->paraptr_pat[i]);

  if (progress != NULL) next_progress_step (progress);

  init_songdata ();

#if !ADT2PLAY
  for (uint8_t i = 0; i < num_channels; i++)
    if (header.chan_set[i] & S3M_CHAN_MUTED)
      channel_flag[i] = false;
#endif // !ADT2PLAY

  if (adjust_tracks)
    song->nm_tracks = max (num_channels, AT_MELODIC_CHANNELS_MAX);

  song->patt_len = S3M_PATTERN_LEN;
  speed = header.i_s != 0 ? header.i_s : 1;
  tempo = header.i_t / 2.5f <= 255 ? header.i_t / 2.5f : 255;
  song->tempo = tempo;
  song->speed = speed;
  song->common_flag |= 0x80;
  import_old_flags ();
  {
    String_t s, t;

    t = asciiz_string (StrToString ((String *)&s, header.title, sizeof (header.title) - 1));
    s = truncate_string ((String *)&t);
    CopyString (song->songname, (String *)&s, sizeof (song->songname) - 1);
    s = NameOnly (_fname);
    CopyString (songdata_title, (String *)&s, sizeof (songdata_title) - 1);
  }
  result_state = 1;
  if (progress != NULL) next_progress_step (progress);

  // import patterns order
  for (uint_least8_t i = 0; i < max (header.ordnum, AT_ORDER_LEN); i++)
    if (data->order[i] < S3M_PATTERNS_MAX)
      song->pattern_order[i] = data->order[i];
    else if (data->order[i] == S3M_ORDER_SKIP)
      song->pattern_order[i] = AT_ORDER_JUMP + i + 1;
    else if (data->order[i] == S3M_ORDER_STOP)
      song->pattern_order[i] = AT_ORDER_JUMP + 0; // acts as `stop'
    else
      goto _err_format;
  result_state = 2;
  if (progress != NULL) next_progress_step (progress);

  // import instruments
  memset (data->c4factor, 0, sizeof (data->c4factor));
  for (uint_least8_t i = 0; i < header.insnum; i++)
    if (data->paraptr_ins[i] != 0)
    {
      s3m_adlib_ins_t insdata;
      String_t s;

      if (fseek (f, (long)data->paraptr_ins[i] << 4, SEEK_SET) != 0) goto _err_fread;
      if (fread (&insdata, sizeof (insdata), 1, f) == 0) goto _err_fread;
      insdata.freq = uint16_LE (insdata.freq);

      if ((insdata.id == S3M_ADLIB_INS_ID) && (insdata.type >= 2) && (insdata.type <= 7))
      {
        data->def_vol[i] = insdata.vol;
        import_standard_instrument_alt (&song->instr_data[i], insdata.fmdata);
        if ((insdata.freq != 0) && (insdata.freq != 8363))
          data->c4factor[i] = find_s3m_scale_factor (insdata.freq, &song->instr_data[i].fine_tune);
      }
      else if (insdata.id == S3M_SAMPLE_INS_ID)
        data->def_vol[i] = insdata.vol;
      else if (insdata.id != 0)
      {
        result = -3;
        goto _err_format;
      }

      if (Length (song->instr_names[i]) > 9) SetLength (song->instr_names[i], 9);
      s = truncate_string (StrToString ((String *)&s, insdata.title, sizeof (insdata.title) - 1));
      if (s.len == 0)
        s = truncate_string (StrToString ((String *)&s, insdata.dosname, sizeof (insdata.dosname)/*-1*/));
      AppendString (song->instr_names[i], (String *)&s, sizeof (song->instr_names[0]) - 1);
    }
  if (progress != NULL) next_progress_step (progress);

  // import patterns
  packed_size = 0;
  for (uint16_t patt = 0; patt < header.patnum; patt++)
    if (data->paraptr_pat[patt] != 0)
    {
      uint16_t packed_blen;
      mem_stream_t stream;
      uint8_t line;

      if (fseek (f, (long)data->paraptr_pat[patt] << 4, SEEK_SET) != 0) goto _err_fread;
      if (fread (&packed_blen, sizeof (packed_blen), 1, f) == 0) goto _err_fread;
      packed_blen = uint16_LE (packed_blen);
      if (packed_blen == 0) continue;
      if (packed_blen < sizeof (packed_blen) + S3M_PATTERN_LEN) goto _err_format;
      packed_blen -= sizeof (packed_blen);

      if (packed_size < packed_blen)
      {
        if (packed_data != NULL)
        {
          void *p = realloc (packed_data, packed_blen);
          if (p == NULL) goto _err_malloc;
          packed_data = p;
        }
        else if ((packed_data = malloc (packed_blen)) == NULL) goto _err_malloc;

        packed_size = packed_blen;
      }
      if (fread (packed_data, packed_blen, 1, f) == 0) goto _err_fread;

      set_mem_stream (&stream, packed_data, packed_blen);
      line = 0;
      do
      {
        uint8_t x;

        if (read_bytes (&x, sizeof (x), &stream)) goto _err_eod;

        if (x != 0)
        {
          s3m_event_t ev;

          ev.chan = x & 0x1F; // 0..S3M_CHANNELS_MAX-1

          if (x & 0x20)
          {
            if (read_bytes (&ev.note, sizeof (ev.note), &stream)) goto _err_eod;
            if (read_bytes (&ev.ins, sizeof (ev.ins), &stream)) goto _err_eod;
          }
          else
          {
            ev.note = S3M_NOTE_EMPTY;
            ev.ins  = S3M_INS_EMPTY;
          }

          if (x & 0x40)
          {
            if (read_bytes (&ev.vol, sizeof (ev.vol), &stream)) goto _err_eod;
          }
          else
            ev.vol = S3M_VOL_EMPTY;

          if (x & 0x80)
          {
            if (read_bytes (&ev.cmd, sizeof (ev.cmd), &stream)) goto _err_eod;
            if (read_bytes (&ev.info, sizeof (ev.info), &stream)) goto _err_eod;
          }
          else
          {
            ev.cmd  = S3M_CMD_EMPTY;
            ev.info = 0;
          }

          if (adjust_tracks && (ev.chan >= song->nm_tracks) && (ev.chan < AT_MELODIC_CHANNELS_MAX))
            song->nm_tracks = ev.chan + 1;
          if (ev.chan < song->nm_tracks)
            import_s3m_event (patt, line, &ev, data->def_vol);
        }
        else line++;
      } while (line != S3M_PATTERN_LEN);
    }
  fix_s3m_commands (data->c4factor, header.patnum);
  if (progress != NULL) next_progress_step (progress);

  result = 0;

_exit:
  if (f != NULL) fclose (f);
  if (data != NULL) free (data);
  if (packed_data != NULL) free (packed_data);
  if (result < 0)
  {
    if (state != NULL) *state = result_state;
    if (error != NULL) *error = result_error;
  }

  DBG_LEAVE (); //EXIT //s3m_file_loader
  return result;

_err_malloc:
  result_error = "Memory allocation failed";
  goto _exit;

_err_fopen:
  result_error = "Failed to open input file";
  goto _exit;

_err_fread:
  result_error = "Failed to read input file";
  goto _exit;

_err_format:
  result_error = "Unknown file format";
  goto _exit;

_err_chan:
  result_error = "No channels to play";
  goto _exit;

_err_eod:
  result_error = "Unexpected end of input data";
  goto _exit;
}
