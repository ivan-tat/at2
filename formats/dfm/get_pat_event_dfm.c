// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_pat_event_dfm (tCHUNK *dst, uint8_t note, uint8_t fx)
{
  uint8_t semitone;

  memset (dst, 0, sizeof (*dst));

  semitone = note & 0x0F;
  if ((semitone >= 1) && (semitone <= 12))
    dst->note = NOTE_PACK (semitone - 1 + (note >> 4) * 12);
  else if (semitone == 15)
    dst->note = NOTE_OFF;
  else
    dst->note = NOTE_EMPTY;

  switch (fx >> 5)
  {
    case 1: // INSTRUMENT CHANGE
      dst->instr_def = INS_PACK (fx & 0x1F);
      break;

    case 2: // SET INSTRUMENT VOLUME
      dst->effect_def = ef_SetInsVolume;
      dst->effect     = (fx & 0x1F) * 2;
      break;

    case 3: // TEMPO CHANGE
      dst->effect_def = ef_SetSpeed;
      dst->effect     = (fx & 0x1F) + 1;
      break;

    case 4: // SLIDE UP
      dst->effect_def = ef_FSlideUpFine;
      dst->effect     = fx & 0x1F;
      break;

    case 5: // SLIDE DOWN
      dst->effect_def = ef_FSlideDownFine;
      dst->effect     = fx & 0x1F;
      break;

    case 7: // END OF PATTERN
      dst->effect_def = ef_PatternBreak;
      break;

    default: break;
  }
}
