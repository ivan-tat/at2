// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_pat_event_rad (tCHUNK *dst,
                               uint8_t byte1, uint8_t byte2, uint8_t param)
{
  uint8_t ins;

  memset (dst, 0, sizeof (*dst));

  ins = INS_GET_RAD (byte1, byte2);
  if (ins != INS_EMPTY_RAD)
    dst->instr_def = INS_PACK (INS_UNPACK_RAD (ins));

  if (IS_NOTE_RAD (byte1))
  {
    uint8_t note = NOTE_UNPACK_RAD (byte1);
    dst->note = NOTE_PACK (note > NOTE_MAX ? NOTE_MAX-1 : note);
  }
  else if (IS_NOTE_OFF_RAD (byte1))
    dst->note = NOTE_OFF;
  else
    dst->note = NOTE_EMPTY;

  switch (byte2 & 0x0F)
  {
    case 1: // PORTAMENTO (FREQUENCY SLIDE) UP
      dst->effect_def = ef_FSlideUp;
      dst->effect     = param;
      break;

    case 2: // PORTAMENTO (FREQUENCY SLIDE) DOWN
      dst->effect_def = ef_FSlideDown;
      dst->effect     = param;
      break;

    case 3: // PORTAMENTO TO NOTE
      dst->effect_def = ef_TonePortamento;
      dst->effect     = param;
      break;

    case 5: // PORTAMENTO TO NOTE WITH VOLUME SLIDE
      if ((param > 0) && (param < 50))
      {
        dst->effect_def = ef_TPortamVolSlide;
        dst->effect     = max (param, 15);

        if (param > 15)
        {
          dst->effect_def2 = ef_TPortamVolSlide;
          dst->effect2     = max (param - 15, 15);
        }
      }
      else if ((param > 50) && (param < 100))
      {
        dst->effect_def = ef_TPortamVolSlide;
        dst->effect     = max (param - 50, 15) * 16;

        if (param - 50 > 15)
        {
          dst->effect_def2 = ef_TPortamVolSlide;
          dst->effect2     = max (param - 50 - 15, 15);
        }
      }
      break;

    case 10:  // VOLUME SLIDE
      if ((param > 0) && (param < 50))
      {
        dst->effect_def = ef_VolSlide;
        dst->effect     = max (param, 15);

        if (param > 15)
        {
          dst->effect_def2 = ef_VolSlide;
          dst->effect2     = max (param - 15, 15);
        }
      }
      else if ((param > 50) && (param < 100))
      {
        dst->effect_def = ef_VolSlide;
        dst->effect     = max (param - 50, 15) * 16;

        if (param - 50 > 15)
        {
          dst->effect_def2 = ef_VolSlide;
          dst->effect2     = max (param - 50 - 15, 15);
        }
      }
      break;

    case 12:  // SET VOLUME
      dst->effect_def = ef_SetInsVolume;
      dst->effect     = param <= 63 ? param : 63;
      break;

    case 13:  // JUMP TO NEXT PATTERN IN ORDER LIST
      dst->effect_def = ef_PatternBreak;
      dst->effect     = param <= 63 ? param : 63;
      break;

    case 15:  // SET SPEED
      dst->effect_def = ef_SetSpeed;
      dst->effect     = param;
      break;

    default: break;
  }

  // specific corrections for RAd-Tracker event
  if (  (dst->effect_def == ef_TonePortamento)
     || (dst->effect_def == ef_TPortamVolSlide))
  {
    if (dst->note == NOTE_OFF) dst->note = NOTE_EMPTY;
    dst->instr_def = INS_EMPTY;
  }
  else if (dst->note == NOTE_EMPTY) dst->instr_def = INS_EMPTY;
}
