// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static const uint8_t pan_lut_fmk[4] = { 1, 0, 2, 0 };

static void get_pat_event_fmk (tCHUNK *dst, pat_event_fmk_t *src, bool stereo)
{
  uint8_t semitone, octave;

  memset (dst, 0, sizeof (*dst));

  dst->instr_def =    (src->ins >= INS_PACK_FMK (0))
                   && (src->ins <= INS_PACK_FMK (INSTRUMENTS_MAX_FMK - 1))
                 ? INS_PACK (INS_UNPACK_FMK (src->ins)) : INS_EMPTY;

  semitone = src->note & 0x0F;
  octave = src->note >> 4;
  if (   (semitone >= 1)
      && (semitone <= 12)
      && (octave < OCTAVES_MAX_FMK))
    dst->note = NOTE_PACK (12 * octave + semitone - 1);
  else
    dst->note = src->note == NOTE_OFF_FMK ? NOTE_OFF : NOTE_EMPTY;

  if (src->vol != VOL_EMPTY_FMK)
  {
    dst->effect_def2 = ef_ForceInsVolume;
    dst->effect2     = 63 - max (src->vol, 63);
  }

  switch (src->cmd)
  {
    case 1: // A: SET SPEED
      if (src->info != 0)
      {
        dst->effect_def = ef_SetSpeed;
        dst->effect     = src->info;
      }
      break;

    case 2: // B: JUMP TO ORDER
      if (src->info < PATTERN_ORDER_LEN_FMK)
      {
        dst->effect_def = ef_PositionJump;
        dst->effect     = src->info;
      }
      break;

    case 3: // C: CARRIER PARAM
      switch (src->info >> 4)
      {
        case 1:
          dst->effect_def = ef_Extended3;
          dst->effect     = (ef_ex3_SetMultipC << 4) + (src->info & 0x0F);
          break;

        case 2:
          dst->effect_def = ef_Extended3;
          dst->effect     = (ef_ex3_SetKslC << 4) + (src->info & 0x03);
          break;

        case 3:
          dst->effect_def = ef_Extended;
          dst->effect     = (ef_ex_SetAttckRateC << 4) + (src->info & 0x0F);
          break;

        case 4:
          dst->effect_def = ef_Extended;
          dst->effect = (ef_ex_SetDecayRateC << 4) + (src->info & 0x0F);
          break;

        case 5:
          dst->effect_def = ef_Extended;
          dst->effect     = (ef_ex_SetSustnLevelC << 4) + (src->info & 0x0F);
          break;

        case 6:
          dst->effect_def = ef_Extended;
          dst->effect     = (ef_ex_SetRelRateC << 4) + (src->info & 0x0F);
          break;

        case 7:
          dst->effect_def = ef_SetWaveform;
          dst->effect     = ((src->info & 7) << 4) + 0x0F;
          break;

        case 8:
          dst->effect_def = ef_Extended;
          dst->effect     = (ef_ex_SetFeedback << 4) + (src->info & 7);
          break;

        default: break;
      }
      break;

    case 4: // D: VOLUME SLIDE
      switch (src->info >> 4)
      {
        case 0: // NORMAL VOLUME SLIDE DOWN
          if ((src->info & 0x0F) == 0)
            dst->effect_def = temp_ef_XFVSlide;
          else
          {
            dst->effect_def = ef_Extended2;
            dst->effect     = (ef_ex2_VolSlideDnXF << 4) + (src->info & 0x0F);
          }
          break;

        case 15: // FINE VOLUME SLIDE DOWN
          dst->effect_def = ef_VolSlideFine;
          dst->effect     = src->info & 0x0F;
          break;

        default: // VOLUME SLIDE UP
          switch (src->info & 0x0F)
          {
            case 0: // NORMAL
              if (src->info >> 4 == 0)
                dst->effect_def = temp_ef_XFVSlide;
              else
              {
                dst->effect_def = ef_Extended2;
                dst->effect     = (ef_ex2_VolSlideUpXF << 4) + (src->info >> 4);
              }
              break;

            case 15: // FINE
              dst->effect_def = ef_VolSlideFine;
              dst->effect     = src->info & 0xF0;
              break;

            default: break;
          }
          break;
      }
      break;

    case 5: // E: SLIDE DOWN
      if (src->info >> 4 < 15)
      { // NORMAL
        dst->effect_def = ef_FSlideDown;
        dst->effect     = src->info;
      }
      else
      { // FINE
        dst->effect_def = ef_FSlideDownFine;
        dst->effect     = src->info & 0x0F;
      }
      break;

    case 6: // F: SLIDE UP
      if (src->info >> 4 < 15)
      { // NORMAL
        dst->effect_def = ef_FSlideUp;
        dst->effect     = src->info;
      }
      else
      { // FINE
        dst->effect_def = ef_FSlideUpFine;
        dst->effect     = src->info & 0x0F;
      }
      break;

    case 7: // G: TONE PORTAMENTO
      dst->effect_def = ef_TonePortamento;
      dst->effect     = src->info;
      break;

    case 8: // H: VIBRATO
      dst->effect_def = ef_Vibrato;
      if ((src->info != 0) && (src->info >> 4 == 0))
        dst->effect = 0x10 + (src->info & 0x0F);
      else if ((src->info != 0) && ((src->info & 0x0F) == 0))
        dst->effect = (src->info & 0xF0) + 1;
      else
        dst->effect = src->info;
      break;

    case 9: // I: RETRIG NOTE
      dst->effect_def = ef_RetrigNote;
      if (src->info != 0) dst->effect = max (src->info * 2, 255);
      break;

    case 10:  // J: ARPEGGIO
      dst->effect_def = temp_ef_Arpeggio;
      dst->effect     = src->info;
      break;

    case 11:  // K: n/a
    case 12:  // L: n/a
      dst->effect = src->info;
      break;

    case 13:  // M: MODULATOR PARAM
      switch (src->info >> 4)
      {
        case 1:
          dst->effect_def = ef_Extended3;
          dst->effect     = (ef_ex3_SetMultipM << 4) + (src->info & 0x0F);
          break;

        case 2:
          dst->effect_def = ef_Extended3;
          dst->effect     = (ef_ex3_SetKslM << 4) + (src->info & 0x03);
          break;

        case 3:
          dst->effect_def = ef_Extended;
          dst->effect     = (ef_ex_SetAttckRateM << 4) + (src->info & 0x0F);
          break;

        case 4:
          dst->effect_def = ef_Extended;
          dst->effect     = (ef_ex_SetDecayRateM << 4) + (src->info & 0x0F);
          break;

        case 5:
          dst->effect_def = ef_Extended;
          dst->effect     = (ef_ex_SetSustnLevelM << 4) + (src->info & 0x0F);
          break;

        case 6:
          dst->effect_def = ef_Extended;
          dst->effect     = (ef_ex_SetRelRateM << 4) + (src->info & 0x0F);
          break;

        case 7:
          dst->effect_def = ef_SetWaveform;
          dst->effect     = 0xF0 + (src->info & 0x07);
          break;

        case 8:
          dst->effect_def = ef_Extended;
          dst->effect     = (ef_ex_SetFeedback << 4) + (src->info & 7);
          break;

        default: break;
      }
      break;

    case 14:  // N: SET VIBRATO/TREMOLO WAVEFORM
    case 15:  // O: n/a
      dst->effect = src->info;
      break;

    case 16:  // P: PATTERN BREAK
      if (src->info < PATTERN_LEN_FMK)
      {
        dst->effect_def = ef_PatternBreak;
        dst->effect     = 10 * (src->info >> 4) + (src->info & 0x0F);
      }
      break;

    case 17:  // Q: n/a
      dst->effect = src->info;
      break;

    case 18:  // R: TREMOLO
      dst->effect_def = ef_Tremolo;
      if ((src->info != 0) && (src->info >> 4 == 0))
        dst->effect = 0x10 + (src->info & 0x0F);
      else if ((src->info != 0) && ((src->info & 0x0F) == 0))
        dst->effect = (src->info & 0xF0) + 1;
      else
        dst->effect = src->info;
      break;

    case 19:  // S: STEREO CONTROL
      if (stereo && (src->info >= 1) && (src->info <= 3))
      {
        dst->effect_def = ef_Extended;
        dst->effect     = (ef_ex_SetPanningPos << 4) + pan_lut_fmk[src->info - 1];
      }
      break;

    case 20:  // T: MODULATOR VOLUME
      dst->effect_def = ef_SetModulatorVol;
      dst->effect     = src->info & 0x3F;
      break;

    case 21:  // U: CARRIER VOLUME
      dst->effect_def = ef_SetCarrierVol;
      dst->effect     = src->info & 0x3F;
      break;

    case 22:  // V: n/a
    case 23:  // W: n/a
    case 24:  // X: n/a
    case 25:  // Y: n/a
    case 26:  // Z: n/a
      dst->effect = src->info;
      break;

    default: break;
  }

  if ((dst->effect_def == 0) && (dst->effect != 0))
    dst->effect = 0;
}
