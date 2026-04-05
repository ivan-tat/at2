// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// FM-Kingtracker song loader (*.fmk)

#define FMK_OCTAVES 8
#define FMK_INSTRUMENTS_MAX 99
#define FMK_MELODIC_CHANNELS_MAX 18
#define FMK_CHANNELS_MAX 20
#define FMK_PATTERN_LEN 64
#define FMK_PATTERNS_MAX 64
#define FMK_ORDER_LEN 255

#define FMK_ID MK_UINT32('F','M','K','!')

// flags
#define FMK_FL_STEREO   0x01
#define FMK_FL_OPL3     0x02
#define FMK_FL_RYTHM    0x04
#define FMK_FL_TREMOLO  0x08  // 4.8 db tremolo
#define FMK_FL_VIBRATO  0x10  // 14 cent vibrato

#pragma pack(push, 1)
typedef struct
{
  uint32_t id;        // ="FMK!"
  char title[28];     // May not be NULL-terminated
  char composer[28];  // May not be NULL-terminated
  uint8_t bytef4;     // =0xF4
  uint8_t version;    // 1=mixed channels, 2=melodic channels only
  uint8_t flags;
  uint8_t base_speed; // Base speed (ticks / second). If set to 0 -> use 50.
  uint8_t init_speed; // Initial speed. If set to 0 -> use 1.
  uint8_t unused[9];
  uint8_t ordlen;     // Length of order
  uint8_t insnum;     // Number of instruments
  uint8_t patnum;     // Number of patterns
  uint8_t panning[5]; // Stereo pan positions for 18 channels, 36 bits total:
                      // bits 2*N-1..2*N-2 (where N=1..18): 0=left, 1=both, 2=right
} fmk_header_t;
#pragma pack(pop)

typedef struct  // all values are little-endian
{
  uint8_t chan_set[FMK_CHANNELS_MAX]; // Channels settings
    // * bits 7..3=OPL-channel number: 1..18, 31=none
    // * bits 2..0=type: 0=melodic, 1=hi-hat, 2=top cymbal, 3=tom-tom, 4=snare drum, 5=bass drum, 6=4-op, 7=unused
    // For version 2 -> channel: 1..18, 31=unused; type: 0=normal, 6=4-op, 7=unused
  uint8_t order[FMK_ORDER_LEN];
  uint16_t offs_desc;
  uint16_t offs_ins[FMK_INSTRUMENTS_MAX];
  uint32_t offs_pat[FMK_PATTERNS_MAX];
} fmk_loader_data_t;

// patterns order
#define FMK_ORDER_STOP  0xFF

// description
#define FMK_DESC_LINE_SIZE 20
#define FMK_DESC_LINES_MAX 24

// instrument
#pragma pack(push, 1)
typedef struct
{
  char dname[12]; // DOS file name padded with spaces (not terminated by NUL)
  char iname[27]; // Instrument name padded with spaces (not terminated by NUL)
  tFM_INST_DATA idata;
} fmk_ins_t;
#pragma pack(pop)

// event
#define FMK_NOTE_OFF    0xFE
#define FMK_NOTE_EMPTY  0xFF
#define FMK_INS_EMPTY   0
#define FMK_VOL_EMPTY   0xFF
#define FMK_CMD_EMPTY   0

typedef struct
{
  uint8_t
    chan, // 1..FMK_CHANNELS_MAX
    note,
    ins,
    vol,
    cmd,
    info;
} fmk_event_t;

static const uint8_t fmk_pan_lut[4] = { 1, 0, 2, 0 };

static void import_fmk_event (uint8_t patt, uint8_t line, fmk_event_t *ev, bool stereo)
{
  tCHUNK a;

  memset (&a, 0, sizeof (a));
  a.instr_def = (ev->ins >= 1) && (ev->ins <= FMK_INSTRUMENTS_MAX) ? ev->ins : AT_INS_EMPTY;

  if (((ev->note & 0x0F) >= 1) && ((ev->note & 0x0F) <= 12) && (ev->note >> 4 < FMK_OCTAVES))
    a.note = 12 * (ev->note >> 4) + (ev->note & 0x0F);
  else
    a.note = ev->note == FMK_NOTE_OFF ? AT_NOTE_OFF : AT_NOTE_EMPTY;

  if (ev->vol != FMK_VOL_EMPTY)
  {
    a.effect_def2 = ef_ForceInsVolume;
    a.effect2     = 63 - max (ev->vol, 63);
  }

  switch (ev->cmd)
  {
    case 1: // A: SET SPEED
      if (ev->info != 0)
      {
        a.effect_def = ef_SetSpeed;
        a.effect     = ev->info;
      }
      break;

    case 2: // B: JUMP TO ORDER
      if (ev->info < FMK_ORDER_LEN)
      {
        a.effect_def = ef_PositionJump;
        a.effect     = ev->info;
      }
      break;

    case 3: // C: CARRIER PARAM
      switch (ev->info >> 4)
      {
        case 1:
          a.effect_def = ef_Extended3;
          a.effect     = (ef_ex3_SetMultipC << 4) + (ev->info & 0x0F);
          break;

        case 2:
          a.effect_def = ef_Extended3;
          a.effect     = (ef_ex3_SetKslC << 4) + (ev->info & 0x03);
          break;

        case 3:
          a.effect_def = ef_Extended;
          a.effect     = (ef_ex_SetAttckRateC << 4) + (ev->info & 0x0F);
          break;

        case 4:
          a.effect_def = ef_Extended;
          a.effect = (ef_ex_SetDecayRateC << 4) + (ev->info & 0x0F);
          break;

        case 5:
          a.effect_def = ef_Extended;
          a.effect     = (ef_ex_SetSustnLevelC << 4) + (ev->info & 0x0F);
          break;

        case 6:
          a.effect_def = ef_Extended;
          a.effect     = (ef_ex_SetRelRateC << 4) + (ev->info & 0x0F);
          break;

        case 7:
          a.effect_def = ef_SetWaveform;
          a.effect     = ((ev->info & 7) << 4) + 0x0F;
          break;

        case 8:
          a.effect_def = ef_Extended;
          a.effect     = (ef_ex_SetFeedback << 4) + (ev->info & 7);
          break;

        default: break;
      }
      break;

    case 4: // D: VOLUME SLIDE
      switch (ev->info >> 4)
      {
        case 0: // NORMAL VOLUME SLIDE DOWN
          if ((ev->info & 0x0F) == 0)
            a.effect_def = temp_ef_XFVSlide;
          else
          {
            a.effect_def = ef_Extended2;
            a.effect     = (ef_ex2_VolSlideDnXF << 4) + (ev->info & 0x0F);
          }
          break;

        case 15: // FINE VOLUME SLIDE DOWN
          a.effect_def = ef_VolSlideFine;
          a.effect     = ev->info & 0x0F;
          break;

        default: // VOLUME SLIDE UP
          switch (ev->info & 0x0F)
          {
            case 0: // NORMAL
              if (ev->info >> 4 == 0)
                a.effect_def = temp_ef_XFVSlide;
              else
              {
                a.effect_def = ef_Extended2;
                a.effect     = (ef_ex2_VolSlideUpXF << 4) + (ev->info >> 4);
              }
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
      if (ev->info >> 4 < 15)
      { // NORMAL
        a.effect_def = ef_FSlideDown;
        a.effect     = ev->info;
      }
      else
      { // FINE
        a.effect_def = ef_FSlideDownFine;
        a.effect     = ev->info & 0x0F;
      }
      break;

    case 6: // F: SLIDE UP
      if (ev->info >> 4 < 15)
      { // NORMAL
        a.effect_def = ef_FSlideUp;
        a.effect     = ev->info;
      }
      else
      { // FINE
        a.effect_def = ef_FSlideUpFine;
        a.effect     = ev->info & 0x0F;
      }
      break;

    case 7: // G: TONE PORTAMENTO
      a.effect_def = ef_TonePortamento;
      a.effect     = ev->info;
      break;

    case 8: // H: VIBRATO
      a.effect_def = ef_Vibrato;
      if ((ev->info != 0) && (ev->info >> 4 == 0))
        a.effect = 0x10 + (ev->info & 0x0F);
      else if ((ev->info != 0) && ((ev->info & 0x0F) == 0))
        a.effect = (ev->info & 0xF0) + 1;
      else
        a.effect = ev->info;
      break;

    case 9: // I: RETRIG NOTE
      a.effect_def = ef_RetrigNote;
      if (ev->info != 0) a.effect = max (ev->info * 2, 255);
      break;

    case 10:  // J: ARPEGGIO
      a.effect_def = temp_ef_Arpeggio;
      a.effect     = ev->info;
      break;

    case 11:  // K: n/a
    case 12:  // L: n/a
      a.effect = ev->info;
      break;

    case 13:  // M: MODULATOR PARAM
      switch (ev->info >> 4)
      {
        case 1:
          a.effect_def = ef_Extended3;
          a.effect     = (ef_ex3_SetMultipM << 4) + (ev->info & 0x0F);
          break;

        case 2:
          a.effect_def = ef_Extended3;
          a.effect     = (ef_ex3_SetKslM << 4) + (ev->info & 0x03);
          break;

        case 3:
          a.effect_def = ef_Extended;
          a.effect     = (ef_ex_SetAttckRateM << 4) + (ev->info & 0x0F);
          break;

        case 4:
          a.effect_def = ef_Extended;
          a.effect     = (ef_ex_SetDecayRateM << 4) + (ev->info & 0x0F);
          break;

        case 5:
          a.effect_def = ef_Extended;
          a.effect     = (ef_ex_SetSustnLevelM << 4) + (ev->info & 0x0F);
          break;

        case 6:
          a.effect_def = ef_Extended;
          a.effect     = (ef_ex_SetRelRateM << 4) + (ev->info & 0x0F);
          break;

        case 7:
          a.effect_def = ef_SetWaveform;
          a.effect     = 0xF0 + (ev->info & 0x07);
          break;

        case 8:
          a.effect_def = ef_Extended;
          a.effect     = (ef_ex_SetFeedback << 4) + (ev->info & 7);
          break;

        default: break;
      }
      break;

    case 14:  // N: SET VIBRATO/TREMOLO WAVEFORM
    case 15:  // O: n/a
      a.effect = ev->info;
      break;

    case 16:  // P: PATTERN BREAK
      if (ev->info < FMK_PATTERN_LEN)
      {
        a.effect_def = ef_PatternBreak;
        a.effect     = 10 * (ev->info >> 4) + (ev->info & 0x0F);
      }
      break;

    case 17:  // Q: n/a
      a.effect = ev->info;
      break;

    case 18:  // R: TREMOLO
      a.effect_def = ef_Tremolo;
      if ((ev->info != 0) && (ev->info >> 4 == 0))
        a.effect = 0x10 + (ev->info & 0x0F);
      else if ((ev->info != 0) && ((ev->info & 0x0F) == 0))
        a.effect = (ev->info & 0xF0) + 1;
      else
        a.effect = ev->info;
      break;

    case 19:  // S: STEREO CONTROL
      if (stereo && (ev->info >= 1) && (ev->info <= 3))
      {
        a.effect_def = ef_Extended;
        a.effect     = (ef_ex_SetPanningPos << 4) + fmk_pan_lut[ev->info - 1];
      }
      break;

    case 20:  // T: MODULATOR VOLUME
      a.effect_def = ef_SetModulatorVol;
      a.effect     = ev->info & 0x3F;
      break;

    case 21:  // U: CARRIER VOLUME
      a.effect_def = ef_SetCarrierVol;
      a.effect     = ev->info & 0x3F;
      break;

    case 22:  // V: n/a
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
  put_chunk (patt, line, ev->chan, &a);
}

typedef struct
{
  uint8_t
    ins     [FMK_CHANNELS_MAX],
    misc    [FMK_CHANNELS_MAX],
    arpg    [FMK_CHANNELS_MAX],
    forcevol[FMK_CHANNELS_MAX],
    volsld  [FMK_CHANNELS_MAX],
    xfvolsld[FMK_CHANNELS_MAX],
    slide   [FMK_CHANNELS_MAX];
  bool first_ins_load[FMK_CHANNELS_MAX];
  bool speed_table_fixed[AT_PATTERNS_MAX];
  struct
  {
    uint8_t
      effect_def,
      effect,
      effect_def2,
      effect2;
  } prev[FMK_CHANNELS_MAX];
} fmk_commands_cache_t;

static void fix_fmk_pattern (uint8_t patt, fmk_commands_cache_t *cache)
{
  uint8_t patt_break;

  memset (cache->prev, 0, sizeof (cache->prev));

  if (!cache->speed_table_fixed[patt])
    for (uint8_t line = 0; line < FMK_PATTERN_LEN; line++)
    {
      for (uint8_t chan = 0; chan < FMK_CHANNELS_MAX; chan++)
      {
        tCHUNK a;

        get_chunk (patt, line, chan + 1, &a);
        if (a.effect_def == 0)
        {
          a.effect_def = ef_SetCustomSpeedTab;
          a.effect     = 0xFA;
          put_chunk (patt, line, chan + 1, &a);
          cache->speed_table_fixed[patt] = true;
        }
        else if (a.effect_def2 == 0)
        {
          a.effect_def2 = ef_SetCustomSpeedTab;
          a.effect2     = 0xFA;
          put_chunk (patt, line, chan + 1, &a);
          cache->speed_table_fixed[patt] = true;
        }
        if (cache->speed_table_fixed[patt]) break;
      }
      if (cache->speed_table_fixed[patt]) break;
    }

  patt_break = FMK_PATTERN_LEN;
  for (uint8_t line = 0; line < FMK_PATTERN_LEN; line++)
    for (uint8_t chan = 0; chan < FMK_CHANNELS_MAX; chan++)
    {
      tCHUNK a;

      get_chunk (patt, line, chan + 1, &a);

      if (a.effect_def == temp_ef_rep)
      {
        a.effect_def = cache->prev[chan].effect_def;
        put_chunk (patt, line, chan + 1, &a);
      }

      if (a.effect_def == temp_ef_XFVSlide)
      {
        a.effect_def = ef_Extended2;
        if (cache->xfvolsld[chan] != 0)
          a.effect = (ef_ex2_VolSlideUpXF << 4) + (cache->volsld[chan] >> 4);
        else
          a.effect = (ef_ex2_VolSlideDnXF << 4) + (cache->volsld[chan] & 0x0F);
        put_chunk (patt, line, chan + 1, &a);
      }

      if (   (a.effect_def == ef_PositionJump)
          || (a.effect_def == ef_PatternBreak))
        patt_break = line;

      if (   (line <= patt_break)
          && (a.instr_def   != 0)
          && (a.instr_def   != cache->ins[chan])
          && (a.effect_def2 != ef_ForceInsVolume))
        cache->forcevol[chan] = 0;

      if ((   (a.effect_def == ef_Extended)
           && (a.effect     == (ef_ex_ExtendedCmd2 << 4) + ef_ex_cmd2_ResetVol))
           || (   (a.effect_def2 == ef_Extended)
               && (a.effect2     == (ef_ex_ExtendedCmd2 << 4) + ef_ex_cmd2_ResetVol)))
        cache->forcevol[chan] = 0;

      if (line <= patt_break)
      {
        if (a.effect_def2 == ef_ForceInsVolume)
          cache->forcevol[chan] = 1;

        if (a.instr_def != 0)
          cache->ins[chan] = a.instr_def;
      }

      if (   (a.instr_def != 0)
          || (   (a.note >= 1)
              && (a.note <= 12*8+1)
              && (cache->ins[chan] != 0)))
        put_chunk (patt, line, chan + 1, &a);

      if (line <= patt_break)
      {
        if (a.effect >> 4 != 0)
          cache->misc[chan] = (a.effect & 0xF0) + (cache->misc[chan] & 0x0F);
        else if (   (a.effect_def == ef_Vibrato)
                 || (a.effect_def == ef_Tremolo))
        {
          a.effect = (cache->misc[chan] & 0xF0) + (a.effect & 0x0F);
          put_chunk (patt, line, chan + 1, &a);
        }

        if ((a.effect & 0x0F) != 0)
          cache->misc[chan] = (cache->misc[chan] & 0xF0) + (a.effect & 0x0F);
        else if (   (a.effect_def == ef_Vibrato)
                 || (a.effect_def == ef_Tremolo))
        {
          a.effect = (a.effect & 0xF0) + (cache->misc[chan] & 0x0F);
          put_chunk (patt, line, chan + 1, &a);
        }

        if (a.effect_def == ef_RetrigNote)
        {
          if (a.effect != 0)
            cache->misc[chan] = a.effect;
          else
          {
            a.effect = cache->misc[chan];
            put_chunk (patt, line, chan + 1, &a);
          }
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

        if (   (a.effect_def == ef_Extended2)
            && (   (a.effect >> 4 == ef_ex2_FreqSlideDnXF)
                || (a.effect >> 4 == ef_ex2_FreqSlideUpXF)))
        {
          if ((a.effect & 0x0F) != 0)
            cache->slide[chan] = a.effect & 0x0F;
          else
          {
            a.effect = (a.effect & 0xF0) + (cache->slide[chan] & 0x0F);
            put_chunk (patt, line, chan + 1, &a);
          }
        }

        if (   (line <= patt_break)
            && (   (a.effect_def == ef_TPortamVolSlide)
                || (a.effect_def == ef_VibratoVolSlide)
                || (a.effect_def == ef_VolSlide)
                || (a.effect_def == ef_VolSlideFine)))
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
            && (   (a.effect >> 4 == ef_ex2_VolSlideDnXF)
                || (a.effect >> 4 == ef_ex2_VolSlideUpXF))
            && ((a.effect & 0x0F) != 0))
          switch (a.effect >> 4)
          {
            case ef_ex2_VolSlideDnXF:
              cache->volsld[chan] = a.effect & 0x0F;
              cache->xfvolsld[chan] = 0;
              break;

            case ef_ex2_VolSlideUpXF:
              cache->volsld[chan] = a.effect & 0xF0;
              cache->xfvolsld[chan] = 1;
              break;

            default: break;
          }
      }

      if (   (   (cache->prev[chan].effect_def == ef_Vibrato)
              || (cache->prev[chan].effect_def == ef_VibratoVolSlide))
          && (a.effect_def != ef_Vibrato)
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

      if (   (a.effect_def2 + a.effect2 == 0)
          && (a.instr_def != 0)
          && (   cache->first_ins_load[chan]
              || ((cache->forcevol[chan] != 0) && (line <= patt_break))))
      {
        if (   (a.effect_def != ef_SetModulatorVol)
            && (a.effect_def != ef_SetCarrierVol))
        {
          a.effect_def2 = ef_Extended;
          a.effect2     = (ef_ex_ExtendedCmd2 << 4) + ef_ex_cmd2_ResetVol;
        }
        else
        {
          a.effect_def2 = a.effect_def;
          a.effect2     = a.effect;
          a.effect_def  = ef_Extended;
          a.effect      = (ef_ex_ExtendedCmd2 << 4) + ef_ex_cmd2_ResetVol;
        }
        put_chunk (patt, line, chan + 1, &a);
        cache->forcevol[chan] = 0;
        cache->first_ins_load[chan] = false;
      }

      cache->prev[chan].effect_def  = a.effect_def;
      cache->prev[chan].effect      = a.effect;
      cache->prev[chan].effect_def2 = a.effect_def2;
      cache->prev[chan].effect2     = a.effect2;

      if (is_4op_chan (chan + 1) && bit_test (_4op_tracks_hi, chan + 1))
      {
        tCHUNK b;

        get_chunk (patt, line, chan + 1, &b);
        if ((a.instr_def == 0) && (b.instr_def != 0))
        {
          a.instr_def = cache->ins[chan] != 0 ? cache->ins[chan] : b.instr_def;
          put_chunk (patt, line, chan + 1, &a);
        }
      }

      if (a.effect_def == temp_ef_Arpeggio)
      {
        tCHUNK b = a;

        b.effect_def = ef_Arpeggio;
        put_chunk (patt, line, chan + 1, &b);
      }

      if (   (   (a.effect_def == ef_SetModulatorVol)
              || (a.effect_def == ef_SetCarrierVol))
          && (a.effect_def2 == ef_ForceInsVolume))
      {
        tCHUNK b = a;

        b.effect_def  = a.effect_def2;
        b.effect      = a.effect2;
        b.effect_def2 = a.effect_def;
        b.effect2     = a.effect;
        put_chunk (patt, line, chan + 1, &b);
      }
    }
}

static void fix_fmk_commands (uint8_t patterns)
{
  uint8_t order, patt;
  fmk_commands_cache_t cache;
  bool patts[AT_PATTERNS_MAX];

  memset (cache.ins, 0, sizeof (cache.ins));
  memset (cache.first_ins_load, true, sizeof (cache.first_ins_load));
  memset (cache.speed_table_fixed, false, sizeof (cache.speed_table_fixed));
  memset (cache.xfvolsld, 0, sizeof (cache.xfvolsld));
  memset (cache.volsld, 0, sizeof (cache.volsld));
  memset (cache.slide, 0, sizeof (cache.slide));
  memset (cache.misc, 0, sizeof (cache.misc));
  memset (cache.arpg, 0, sizeof (cache.arpg));
  memset (cache.forcevol, 0, sizeof (cache.forcevol));
  memset (patts, false, sizeof (patts));

  order = 0;
  patt = AT_PATTERNS_MAX;
  do
  {
    uint8_t x = songdata.pattern_order[order++];

    if (x < AT_PATTERNS_MAX)
    {
      fix_fmk_pattern (x, &cache);
      patts[x] = true;
      patt = x;
    }
    order++;
  } while ((patt < patterns) && (order < AT_ORDER_LEN));

  for (patt = 0; patt < patterns; patt++)
    if (!patts[patt]) fix_fmk_pattern (patt, &cache);
}

static void truncate_fmk_string (const uint8_t **end_ptr, uint8_t *len)
{
  uint8_t l = *len;
  if (l != 0) // try to truncate string
  {
    const uint8_t *p = *end_ptr - 1;
    while ((l != 0) && ((*p == ' ') || (*p == (uint8_t) '\xFF')))
    {
      p--;
      l--;
    }
    *end_ptr = p + 1;
    *len = l;
  }
}

static bool import_fmk_description (void **output, const uint8_t *input, uint8_t input_lines, char **error)
{
  bool result = true;
  char *result_error = NULL;
  uint8_t *out = NULL;
  size_t out_size = 0;
  int_least16_t first = -1, last = -1;

  // pass 1/2: determine text data size (without NULL-terminators) and non-empty lines range
  for (uint8_t line = 0; line < input_lines; line++)
  {
    const uint8_t *end_ptr = &input[FMK_DESC_LINE_SIZE * (line + 1)];
    uint8_t len = FMK_DESC_LINE_SIZE;

    truncate_fmk_string (&end_ptr, &len);
    if (len != 0)
    {
      out_size += len;
      if (first < 0) first = line;
      last = line;
    }
  }

  if (out_size != 0)
  {
    uint8_t lines_count = last - first + 1;
    fast_str_t *lines;
    uint8_t *line_start;

    // sizeof (song_desc_s) + NULL-terminators for found lines
    out_size += sizeof (size_t) + sizeof (fast_str_t) * lines_count + lines_count;
    if ((out = malloc (out_size)) == NULL) goto _err_malloc;

    *((size_t *)out) = lines_count; // =song_desc_s.count
    lines = (fast_str_t *)(&out[sizeof (size_t)]);  // =&out[offsetof (song_desc_s, lines)]
    line_start = (uint8_t *)&lines[lines_count];  // =&out[sizeof (song_desc_s)]

    // pass 2/2: store lines
    for (uint8_t line = 0; line < lines_count; line++)
    {
      const uint8_t *end_ptr = &input[FMK_DESC_LINE_SIZE * (first + line + 1)];
      uint8_t len = FMK_DESC_LINE_SIZE;

      truncate_fmk_string (&end_ptr, &len);
      if (len != 0) memcpy (line_start, end_ptr - len, len);
      line_start[len] = '\0';
      lines[line].len = len;
      lines[line].data = line_start;
      line_start += len + 1;
    }
  }

  result = false;

_exit:
  if (result)
  {
    if (out != NULL) free (out);
    if (error != NULL) *error = result_error;
  }
  else *output = out;

  return result;

_err_malloc:
  result_error = "Memory allocation failed";
  goto _exit;
}

// In:
//   * `desc', `progress', `state' and `error' may be NULL.
//
// On success:
//   * Return value is 0.
//   * `desc' (if set) is set to allocated song description structure.
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
int8_t fmk_file_loader (const String *_fname, void **desc, progress_callback_t *progress, uint8_t *state, char **error)
{
  tFIXED_SONGDATA *song = &songdata;
  int8_t result = -1;
  void *result_desc = NULL;
  uint8_t result_state = 0;
  char *result_error = NULL;
  FILE *f = NULL;
  fmk_header_t header;
  fmk_loader_data_t *data = NULL;
  void *desc_buf = NULL;
  bool stereo;
  uint8_t num_channels;
  void *packed_data = NULL;
  size_t packed_size;
  char fname[255+1];

  DBG_ENTER ("fmk_file_loader");

  if (progress != NULL)
  {
    snprintf (progress->msg, sizeof (progress->msg), "Reading file...");
    progress->num_steps = 5;
    progress->value = 1;
    progress->update (progress, 0, -1);
  }

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "rb")) == NULL) goto _err_fopen;

  if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;

  result = -2;

  if ((header.id != FMK_ID) || (header.bytef4 != 0xF4)) goto _err_format;

  result = -3;

  if (   ((header.version != 1) && (header.version != 2))
      || (header.insnum > FMK_INSTRUMENTS_MAX)
      || (header.patnum > FMK_PATTERNS_MAX)) goto _err_version;

  result = -4;

  if ((data = malloc (sizeof (*data))) == NULL) goto _err_malloc;

  // import channels set
  if (fread (data->chan_set, sizeof (data->chan_set[0])
                             * (header.version == 1 ? FMK_CHANNELS_MAX
                                                    : FMK_MELODIC_CHANNELS_MAX), 1, f) == 0) goto _err_fread;
  if (header.version == 2)  // disable last 2 channels
  {
    data->chan_set[FMK_CHANNELS_MAX - 2] = 0xFF;
    data->chan_set[FMK_CHANNELS_MAX - 1] = 0xFF;
  }
  // find playable channels
  for (num_channels = FMK_CHANNELS_MAX; num_channels >= 1; num_channels--)
    if ((data->chan_set[num_channels - 1] & 7) != 7) break;
  if (num_channels == 0) goto _err_chan;

  // read variable header data
  if (   (header.ordlen != 0)
      && (fread (data->order, sizeof (data->order[0]) * header.ordlen, 1, f) == 0)) goto _err_fread;
  if (fread (&data->offs_desc, sizeof (data->offs_desc), 1, f) == 0) goto _err_fread;
  if (   (header.insnum != 0)
      && (fread (data->offs_ins, sizeof (data->offs_ins[0]) * header.insnum, 1, f) == 0)) goto _err_fread;
  if (   (header.patnum != 0)
      && (fread (data->offs_pat, sizeof (data->offs_pat[0]) * header.patnum, 1, f) == 0)) goto _err_fread;

  data->offs_desc = uint16_LE (data->offs_desc);
  for (uint8_t i = 0; i < header.insnum; i++) data->offs_ins[i] = uint16_LE (data->offs_ins[i]);
  for (uint8_t i = 0; i < header.patnum; i++) data->offs_pat[i] = uint32_LE (data->offs_pat[i]);

  init_songdata ();
  stereo = (header.flags & FMK_FL_STEREO) != 0;

  for (uint8_t chan = 0; chan < num_channels; chan++)
    if ((data->chan_set[chan] & 7) == 6)
      switch (chan)
      {
        case 0:  case 1:  song->flag_4op |= 0x01; break;
        case 2:  case 3:  song->flag_4op |= 0x02; break;
        case 4:  case 5:  song->flag_4op |= 0x04; break;
        case 9:  case 10: song->flag_4op |= 0x08; break;
        case 11: case 12: song->flag_4op |= 0x10; break;
        case 13: case 14: song->flag_4op |= 0x20; break;
        default: break;
      }

  if (adjust_tracks)
    song->nm_tracks = num_channels;
  else if (song->nm_tracks < FMK_MELODIC_CHANNELS_MAX)
    song->nm_tracks = FMK_MELODIC_CHANNELS_MAX;

  song->patt_len = FMK_PATTERN_LEN;
  speed = header.init_speed != 0 ? header.init_speed : 1;
  tempo = header.base_speed != 0 ? header.base_speed : 50;
  song->tempo = tempo;
  song->speed = speed;
  song->common_flag |= 0x01 | 0x02 | 0x80;
  if (stereo) song->common_flag |= 0x20;
  if (header.flags & FMK_FL_TREMOLO) song->common_flag |= 0x08;
  if (header.flags & FMK_FL_VIBRATO) song->common_flag |= 0x10;
  import_old_flags ();
  if (stereo) // must be after a call to `import_old_flags()'
    for (uint8_t chan = 0; chan < FMK_MELODIC_CHANNELS_MAX; chan++)
      song->lock_flags[chan] |= fmk_pan_lut[(header.panning[chan >> 2] >> ((chan & 3) << 1)) & 3];
  {
    String_t s;

    s = truncate_string (StrToString ((String *)&s, header.title, sizeof (header.title)/*-1*/));
    CopyString (song->songname, (String *)&s, sizeof (song->songname) - 1);
    s = truncate_string (StrToString ((String *)&s, header.composer, sizeof (header.composer)/*-1*/));
    CopyString (song->composer, (String *)&s, sizeof (song->composer) - 1);
    s = NameOnly (_fname);
    CopyString (songdata_title, (String *)&s, sizeof (songdata_title) - 1);
  }

  result_state = 1;
  if (progress != NULL) next_progress_step (progress);

  // import patterns order
  for (uint8_t i = 0; i < max (header.ordlen, AT_ORDER_LEN); i++)
    if (data->order[i] < FMK_PATTERNS_MAX)
      song->pattern_order[i] = data->order[i];
    else if (data->order[i] == FMK_ORDER_STOP)
      song->pattern_order[i] = AT_ORDER_JUMP + 0; // acts as `stop'
    else
      goto _err_format;
  result_state = 2;
  if (progress != NULL) next_progress_step (progress);

  // import description
  if ((desc != NULL) && (data->offs_desc != 0))
  {
    uint8_t num_lines;

    if (fseek (f, data->offs_desc, SEEK_SET) != 0) goto _err_fread;
    if (fread (&num_lines, sizeof (num_lines), 1, f) == 0) goto _err_fread;
    if (num_lines != 0)
    {
      size_t size;

      if (num_lines > FMK_DESC_LINES_MAX) goto _err_format;
      size = FMK_DESC_LINE_SIZE * num_lines;
      if ((desc_buf = malloc (size)) == NULL) goto _err_malloc;
      if (fread (desc_buf, size, 1, f) == 0) goto _err_fread;
      if (import_fmk_description (&result_desc, desc_buf, num_lines, &result_error)) goto _exit;
      free (desc_buf);
      desc_buf = NULL;
    }
  }
  if (progress != NULL) next_progress_step (progress);

  // import instruments
  for (uint8_t i = 0; i < header.insnum; i++)
    if (data->offs_ins[i] != 0)
    {
      fmk_ins_t insdata;
      String_t s;

      if (fseek (f, data->offs_ins[i], SEEK_SET) != 0) goto _err_fread;
      if (fread (&insdata, sizeof (insdata), 1, f) == 0) goto _err_fread;

      if (Length (song->instr_names[i]) > 9) SetLength (song->instr_names[i], 9);
      s = truncate_string (StrToString ((String *)&s, insdata.iname, sizeof (insdata.iname)/*-1*/));
      if (s.len == 0)
        s = truncate_string (StrToString ((String *)&s, insdata.dname, sizeof (insdata.dname)/*-1*/));
      AppendString (song->instr_names[i], (String *)&s, sizeof (song->instr_names[0]) - 1);

      import_fin_instrument_alt (&song->instr_data[i], &insdata.idata);
    }
  if (progress != NULL) next_progress_step (progress);

  // import patterns
  packed_size = 0;
  for (uint8_t patt = 0; patt < header.patnum; patt++)
    if (data->offs_pat[patt] != 0)
    {
      uint16_t packed_blen;
      mem_stream_t stream;
      uint8_t line;

      if (fseek (f, (long)data->offs_pat[patt], SEEK_SET) != 0)  goto _err_fread;
      if (fread (&packed_blen, sizeof (packed_blen), 1, f) == 0) goto _err_fread;
      packed_blen = uint16_LE (packed_blen);
      if (packed_blen == 0) continue;
      if (packed_blen < FMK_PATTERN_LEN) goto _err_format;

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
          fmk_event_t ev;

          ev.chan = x & 0x1F; // 1..FMK_CHANNELS_MAX

          if (x & 0x20)
          {
            if (read_bytes (&ev.note, sizeof (ev.note), &stream)) goto _err_eod;
            if (read_bytes (&ev.ins, sizeof (ev.ins), &stream)) goto _err_eod;
          }
          else
          {
            ev.note = FMK_NOTE_EMPTY;
            ev.ins  = FMK_INS_EMPTY;
          }

          if (x & 0x40)
          {
            if (read_bytes (&ev.vol, sizeof (ev.vol), &stream)) goto _err_eod;
          }
          else
            ev.vol = FMK_VOL_EMPTY;

          if (x & 0x80)
          {
            if (read_bytes (&ev.cmd, sizeof (ev.cmd), &stream)) goto _err_eod;
            if (read_bytes (&ev.info, sizeof (ev.info), &stream)) goto _err_eod;
          }
          else
          {
            ev.cmd  = FMK_CMD_EMPTY;
            ev.info = 0;
          }

          if ((ev.chan >= 1) && (ev.chan <= FMK_CHANNELS_MAX))
            import_fmk_event (patt, line, &ev, stereo);
        }
        else line++;
      } while (line != FMK_PATTERN_LEN);
    }
  fix_fmk_commands (header.patnum);
  if (progress != NULL) next_progress_step (progress);

  result = 0;

_exit:
  if (f != NULL) fclose (f);
  if (data != NULL) free (data);
  if (desc_buf != NULL) free (desc_buf);
  if (packed_data != NULL) free (packed_data);
  if (result < 0)
  {
    if (result_desc != NULL) free (result_desc);
    if (state != NULL) *state = result_state;
    if (error != NULL) *error = result_error;
  }
  else
    *desc = result_desc;

  DBG_LEAVE (); //EXIT //fmk_file_loader
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

_err_version:
  result_error = "Unsupported file format version";
  goto _exit;

_err_chan:
  result_error = "No channels to play";
  goto _exit;

_err_eod:
  result_error = "Unexpected end of input data";
  goto _exit;
}
