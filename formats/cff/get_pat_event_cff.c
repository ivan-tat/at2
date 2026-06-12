// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_pat_event_cff (tCHUNK *dst, pat_event_cff_t *src)
{
  memset (dst, 0, sizeof (*dst));

  if (src->note == NOTE_EMPTY_CFF)
    dst->note = NOTE_EMPTY;
  else if (src->note <= NOTE_PACK_CFF (NOTE_MAX_CFF))
  {
    unsigned note = NOTE_UNPACK_CFF (src->note);

    if (fix_c_note_bug) note++; // this is not needed for BoomTracker 4.0

    if (note > NOTE_MAX) note = NOTE_MAX;
    dst->note = NOTE_PACK (note);
  }
  else if (src->note == NOTE_OFF_CFF) // key-off
    dst->note = NOTE_OFF;
  else
    dst->note = NOTE_EMPTY;

  switch (src->fx)
  {
    case 'A': // SET SPEED
    if (src->param > 0)
    {
      dst->effect_def = ef_SetSpeed;
      dst->effect     = src->param;
    }
    break;

    case 'B': // SET CARRIER WAVEFORM
      if (src->param < 4)
      {
        dst->effect_def = ef_SetWaveform;
        dst->effect     = src->param << 4;
      }
      break;

    case 'C': // SET MODULATOR VOLUME
      dst->effect_def = ef_SetModulatorVol;
      dst->effect     = src->param < 64 ? 63 - src->param : 0;
      break;

    case 'D': // VOLUME SLIDE UP/DOWN
      dst->effect_def = ef_VolSlide;
      dst->effect     = src->param;
      break;

    case 'E': // SLIDE DOWN
      if (src->param != 0)
      {
        dst->effect_def = ef_FSlideDown;
        dst->effect     = src->param;
      }
      break;

    case 'F': // SLIDE UP
      if (src->param != 0)
      {
        dst->effect_def = ef_FSlideUp;
        dst->effect     = src->param;
      }
      break;

    case 'G': // SET CARRIER VOLUME
      dst->effect_def = ef_SetCarrierVol;
      dst->effect     = src->param < 64 ? 63 - src->param : 0;
      break;

    case 'H': // SET TEMPO
      if (src->param > 0)
      {
        uint8_t param = src->param <= 21 ? 125 : src->param;
        uint_least32_t period = UINT32_C (1412926) / (param >> 1);

        dst->effect_def = ef_SetTempo;
        for (dst->effect = 1; (PIT_FREQ_MAX / dst->effect > period) && (dst->effect < 255); dst->effect++);
      }
      break;

    case 'I': // SET INSTRUMENT
      if (src->param < INSTRUMENTS_MAX_CFF)
      {
        dst->effect_def = ef_Extended;
        dst->effect     = (ef_ex_ExtendedCmd2 << 4) + ef_ex_cmd2_ResetVol;
        dst->instr_def  = INS_PACK (src->param);
      }
      break;

    case 'J': // ARPEGGIO
      dst->effect_def = ef_Arpeggio;
      dst->effect     = src->param;
      break;

    case 'K': // JUMP TO ORDER
      dst->effect_def = ef_PositionJump;
      dst->effect     = src->param >= PATTERN_ORDER_LEN_CFF ? PATTERN_ORDER_LEN_CFF - 1 : src->param;
      break;

    case 'L': // JUMP TO NEXT PATTERN IN ORDER
      dst->effect_def = ef_PatternBreak;
      break;

    case 'M': // SET TREMOLO HIGHER / SET VIBRATO DEEPER
      dst->effect_def  = ef_Extended;
      dst->effect      = (ef_ex_SetTremDepth << 4) + (src->param & 0xF0 ? 1 : 0);
      dst->effect_def2 = ef_Extended;
      dst->effect2     = (ef_ex_SetVibDepth << 4) + (src->param & 0x0F ? 1 : 0);
      break;

    default: break;
  }
}
