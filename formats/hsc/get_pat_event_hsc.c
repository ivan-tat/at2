// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_pat_event_hsc (tCHUNK *dst, pat_event_hsc_t src, bool fix_note_bug)
{
  uint8_t command = src.command, param = src.param;

  memset (dst, 0, sizeof (*dst));

  if (command & CMD_INS_FLAG_HSC)
  {
    uint8_t ins = param & 0x7F; // 0..INSTRUMENTS_MAX_HSC-1

    dst->note       = NOTE_OFF;
    dst->instr_def  = INS_PACK (ins);
    dst->effect_def = ef_Extended;
    dst->effect     = (ef_ex_ExtendedCmd2 << 4) + ef_ex_cmd2_ResetVol;
  }
  else
  {
    uint8_t data;

    if (   (command >= CMD_NOTE_PACK_HSC (0))
        && (command <= CMD_NOTE_PACK_HSC (NOTE_MAX_HSC))) // regular note
    {
      uint8_t note = CMD_NOTE_UNPACK_HSC (command);

      if (note <= NOTE_MAX)
        dst->note = NOTE_PACK (note + (fix_note_bug && (note < NOTE_MAX) ? 1 : 0));
      else
        dst->note = NOTE_EMPTY;
    }
    else if (command == CMD_NOTE_OFF_HSC)
      dst->note = NOTE_OFF;
    else
      dst->note = NOTE_EMPTY;

    data = param & 0x0F;

    switch (param >> 4)
    {
      case 0: // GLOBAL COMMAND
        switch (data)
        {
          case 1: // PATTERN BREAK
            dst->effect_def = ef_PatternBreak;
            break;

          case 2: // SLIDE GLOBAL VOLUME UP
            dst->effect_def = ef_Extended2;
            dst->effect     = (ef_ex2_GlVolSlideUp << 4) + data;  // TODO: check
            break;

          case 3: // SLIDE GLOBAL VOLUME DOWN
            dst->effect_def = ef_Extended2;
            dst->effect     = (ef_ex2_GlVolSlideDn << 4) + data;  // TODO: check
            break;

          case 4: // SET GLOBAL VOLUME TO 0
            dst->effect_def = ef_SetGlobalVolume;
            dst->effect     = 0;
            break;

          case 5: // PERCUSSION MODE ON (N/A)
          case 6: // PERCUSSION MODE OFF (N/A)
          default: break;
        }
        break;

      case 1: // MANUAL SLIDE UP
        dst->effect_def = ef_Extended2;
        dst->effect     = (ef_ex2_FineTuneUp << 4) + max (data + 1, 15);
        break;

      case 2: // MANUAL SLIDE DOWN
        dst->effect_def = ef_Extended2;
        dst->effect     = (ef_ex2_FineTuneDown << 4) + max (data + 1, 15);
        break;

      case 5: // SET PERCUSSION INSTRUMENT (N/A)
        break;

      case 6: // SET FEEDBACK
        dst->effect_def = ef_Extended;
        dst->effect     = (ef_ex_SetFeedback << 4) + (data & 0x07);
        break;

      case 10:  // SET CARRIER VOLUME
        dst->effect_def = ef_SetCarrierVol;
        dst->effect     = 63 - data * 4;
        break;

      case 11:  // SET MODULATOR VOLUME
        dst->effect_def = ef_SetModulatorVol;
        dst->effect     = 63 - data * 4;
        break;

      case 12:  // SET INSTRUMENT VOLUME
        dst->effect_def = ef_SetInsVolume;
        dst->effect     = 63 - data * 4;
        break;

      case 13:  // POSITION JUMP
        dst->effect_def = ef_PositionJump;
        dst->effect     = data;
        break;

      case 15:  // SET SPEED
        dst->effect_def = ef_SetSpeed;
        dst->effect     = data + 1;
        break;

      default: break;
    }
  }
}
