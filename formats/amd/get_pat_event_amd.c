// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_pat_event_amd (tCHUNK *dst, uint8_t a, uint8_t b, uint8_t param)
{
  uint8_t octave, tone, ins;

  memset (dst, 0, sizeof (*dst));

  octave = (a >> 1) & 7; // 0..7
  tone = a >> 4;  // 0..15
  if ((octave == 0) && (tone == 0))
    dst->note = NOTE_EMPTY;
  else if ((tone >= 1) && (tone <= 12))
    dst->note = NOTE_PACK (NOTE_UNPACK_AMD (12 * octave + tone));
  else
    dst->note = NOTE_EMPTY;

  ins = (b >> 4) + ((a & 1) << 4); // 0..31
  if (ins == 0)
    dst->instr_def = INS_EMPTY;
  else if (INS_UNPACK_AMD (ins) < INSTRUMENTS_MAX_AMD)
    dst->instr_def = INS_PACK (INS_UNPACK_AMD (ins));
  else
    dst->instr_def = INS_EMPTY;

  // specific corrections for Amusic event
  if (dst->note == NOTE_EMPTY) dst->instr_def = INS_EMPTY;

  param &= 0x7F;
  switch (b & 0x0F)
  {
    case 0: // ARPEGGIO
      dst->effect_def = ef_Arpeggio;
      dst->effect     = dec2hex (param);
      break;

    case 1: // SLIDE FREQUENCY UP
      dst->effect_def = ef_FSlideUp;
      dst->effect     = param;
      break;

    case 2: // SLIDE FREQUENCY DOWN
      dst->effect_def = ef_FSlideDown;
      dst->effect     = param;
      break;

    case 3: // SET CARRIER/MODULATOR INTENSITY
      if ((param >= 10) && (param <= 99))
      {
        dst->effect_def = ef_SetCarrierVol;
        dst->effect     = 7 * (param / 10); // 7..63
      }
      else if ((param % 10) != 0)
      {
        dst->effect_def = ef_SetModulatorVol;
        dst->effect     = 7 * (param % 10); // 0..63
      }
      break;

    case 4: // SET THE VOLUME
      dst->effect_def = ef_SetInsVolume;
      dst->effect     = param <= 63 ? param : 63;
      break;

    case 5: // JUMP INTO PATTERN
      dst->effect_def = ef_PositionJump;
      dst->effect     = param <= 99 ? param : 99;
      break;

    case 6: // PATTERNBREAK
      dst->effect_def = ef_PatternBreak;
      dst->effect     = param <= 63 ? param : 63;
      break;

    case 7: // SET SONGSPEED
      if (param < 99)
      {
        if ((param >= 1) && (param <= 31))
        {
          dst->effect_def = ef_SetSpeed;
          dst->effect     = param;
        }
        else
        {
          dst->effect_def = ef_SetTempo;
          dst->effect     = param == 0 ? 18 : param;
        }
      }
      break;

    case 8: // TONEPORTAMENTO
      dst->effect_def = ef_TonePortamento;
      dst->effect     = param;
      break;

    case 9: // EXTENDED COMMAND
      switch (param / 10)
      {
        case 0: // DEFINE CELL-TREMOLO
          if ((param % 10) <= 1)
          {
            dst->effect_def = ef_Extended;
            dst->effect     = (ef_ex_SetTremDepth << 4) + (param % 10);
          }
          break;

        case 1: // DEFINE CELL-VIBRATO
          if ((param % 10) <= 1)
          {
            dst->effect_def = ef_Extended;
            dst->effect     = (ef_ex_SetVibDepth << 4) + (param % 10);
          }
          break;

        case 2: // INCREASE VOLUME FAST
          dst->effect_def = ef_VolSlide;
          dst->effect     = 16 * (param % 10);
          break;

        case 3: // DECREASE VOLUME FAST
          dst->effect_def = ef_VolSlide;
          dst->effect     = param % 10;
          break;

        case 4: // INCREASE VOLUME FINE
          dst->effect_def = ef_Extended2;
          dst->effect     = (ef_ex2_VolSlideUpXF << 4) + (param % 10);
          break;

        case 5: // DECREASE VOLUME FINE
          dst->effect_def = ef_Extended2;
          dst->effect     = (ef_ex2_VolSlideDnXF << 4) + (param % 10);
          break;

        default:
          break;
      }
      break;

    default:
      break;
  }
}
