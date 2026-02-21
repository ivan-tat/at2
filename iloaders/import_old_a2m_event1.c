// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#define fx_Arpeggio          0x00
#define fx_FSlideUp          0x01
#define fx_FSlideDown        0x02
#define fx_FSlideUpFine      0x03
#define fx_FSlideDownFine    0x04
#define fx_TonePortamento    0x05
#define fx_TPortamVolSlide   0x06
#define fx_Vibrato           0x07
#define fx_VibratoVolSlide   0x08
#define fx_SetOpIntensity    0x09
#define fx_SetInsVolume      0x0A
#define fx_PatternBreak      0x0B
#define fx_PatternJump       0x0C
#define fx_SetTempo          0x0D
#define fx_SetTimer          0x0E
#define fx_Extended          0x0F
#define fx_ex_DefAMdepth     0x00
#define fx_ex_DefVibDepth    0x01
#define fx_ex_DefWaveform    0x02
#define fx_ex_ManSlideUp     0x03
#define fx_ex_ManSlideDown   0x04
#define fx_ex_VSlideUp       0x05
#define fx_ex_VSlideDown     0x06
#define fx_ex_VSlideUpFine   0x07
#define fx_ex_VSlideDownFine 0x08
#define fx_ex_RetrigNote     0x09
#define fx_ex_SetAttckRate   0x0A
#define fx_ex_SetDecayRate   0x0B
#define fx_ex_SetSustnLevel  0x0C
#define fx_ex_SetReleaseRate 0x0D
#define fx_ex_SetFeedback    0x0E
#define fx_ex_ExtendedCmd    0x0F

// patt: 0..max_patterns-1
// line: 0..255
// chan: 1..20
static void import_old_a2m_event1 (uint8_t patt, uint8_t line, uint8_t chan,
                                   const tOLD_CHUNK old_chunk,
                                   bool processing_whole_song)
{
  tCHUNK chunk;

  chan--; // 0..19

  memset (&chunk, 0, sizeof (chunk));
  chunk.note       = old_chunk.note;
  chunk.instr_def  = old_chunk.instr_def;
  chunk.effect_def = old_chunk.effect_def;
  chunk.effect     = old_chunk.effect;

  switch (old_chunk.effect_def)
  {
    case fx_Arpeggio:        chunk.effect_def = ef_Arpeggio; break;
    case fx_FSlideUp:        chunk.effect_def = ef_FSlideUp; break;
    case fx_FSlideDown:      chunk.effect_def = ef_FSlideDown; break;
    case fx_FSlideUpFine:    chunk.effect_def = ef_FSlideUpFine; break;
    case fx_FSlideDownFine:  chunk.effect_def = ef_FSlideDownFine; break;
    case fx_TonePortamento:  chunk.effect_def = ef_TonePortamento; break;
    case fx_TPortamVolSlide: chunk.effect_def = ef_TPortamVolSlide; break;
    case fx_Vibrato:         chunk.effect_def = ef_Vibrato; break;
    case fx_VibratoVolSlide: chunk.effect_def = ef_VibratoVolSlide; break;
    case fx_SetInsVolume:    chunk.effect_def = ef_SetInsVolume; break;
    case fx_PatternJump:     chunk.effect_def = ef_PositionJump; break;
    case fx_PatternBreak:    chunk.effect_def = ef_PatternBreak; break;
    case fx_SetTempo:        chunk.effect_def = ef_SetSpeed; break;
    case fx_SetTimer:        chunk.effect_def = ef_SetTempo; break;

    case fx_SetOpIntensity:
      if ((old_chunk.effect & 0xF0) != 0)
      {
        chunk.effect_def = ef_SetCarrierVol;
        chunk.effect = ((old_chunk.effect & 0xF0) >> 2) + 3;
      }
      else if ((old_chunk.effect & 0x0F) != 0)
      {
        chunk.effect_def = ef_SetModulatorVol;
        chunk.effect = ((old_chunk.effect & 0x0F) << 2) + 3;
      }
      else
        chunk.effect_def = 0;
      break;

    case fx_Extended:
      switch (old_chunk.effect >> 4)
      {
        case fx_ex_DefAMdepth:
          chunk.effect_def = ef_Extended;
          chunk.effect = (ef_ex_SetTremDepth << 4) + (old_chunk.effect & 0x0F);
          break;

        case fx_ex_DefVibDepth:
          chunk.effect_def = ef_Extended;
          chunk.effect = (ef_ex_SetVibDepth << 4) + (old_chunk.effect & 0x0F);
          break;

        case fx_ex_DefWaveform:
          chunk.effect_def = ef_SetWaveform;
          switch (old_chunk.effect & 0x0F)
          {
            case 0: case 1: case 2: case 3: chunk.effect = ((old_chunk.effect & 0x0F) << 4) + 0x0F; break;
            case 4: case 5: case 6: case 7: chunk.effect = 0xF0 + (old_chunk.effect & 0x0F) - 4; break;
            default: break;
          }
          break;

        case fx_ex_VSlideUp:
          chunk.effect_def = ef_VolSlide;
          chunk.effect = (old_chunk.effect & 0x0F) << 4;
          break;

        case fx_ex_VSlideDown:
          chunk.effect_def = ef_VolSlide;
          chunk.effect = old_chunk.effect & 0x0F;
          break;

        case fx_ex_VSlideUpFine:
          chunk.effect_def = ef_VolSlideFine;
          chunk.effect = (old_chunk.effect & 0x0F) << 4;
          break;

        case fx_ex_VSlideDownFine:
          chunk.effect_def = ef_VolSlideFine;
          chunk.effect = old_chunk.effect & 0x0F;
          break;

        case fx_ex_ManSlideUp:
          chunk.effect_def = ef_Extended2;
          chunk.effect = (ef_ex2_FineTuneUp << 4) + (old_chunk.effect & 0x0F);
          break;

        case fx_ex_ManSlideDown:
          chunk.effect_def = ef_Extended2;
          chunk.effect = (ef_ex2_FineTuneDown << 4) + (old_chunk.effect & 0x0F);
          break;

        case fx_ex_RetrigNote:
          chunk.effect_def = ef_RetrigNote;
          chunk.effect = (old_chunk.effect & 0x0F) + 1;
          break;

        case fx_ex_SetAttckRate:
          chunk.effect_def = ef_Extended;
          chunk.effect = ((adsr_carrier[chan] ? ef_ex_SetAttckRateC : ef_ex_SetAttckRateM) << 4) + (old_chunk.effect & 0x0F);
          break;

        case fx_ex_SetDecayRate:
          chunk.effect_def = ef_Extended;
          chunk.effect = ((adsr_carrier[chan] ? ef_ex_SetDecayRateC : ef_ex_SetDecayRateM) << 4) + (old_chunk.effect & 0x0F);
          break;

        case fx_ex_SetSustnLevel:
          chunk.effect_def = ef_Extended;
          chunk.effect = ((adsr_carrier[chan] ? ef_ex_SetSustnLevelC : ef_ex_SetSustnLevelM) << 4) + (old_chunk.effect & 0x0F);
          break;

        case fx_ex_SetReleaseRate:
          chunk.effect_def = ef_Extended;
          chunk.effect = ((adsr_carrier[chan] ? ef_ex_SetRelRateC : ef_ex_SetRelRateM) << 4) + (old_chunk.effect & 0x0F);
          break;

        case fx_ex_SetFeedback:
          chunk.effect_def = ef_Extended;
          chunk.effect = (ef_ex_SetFeedback << 4) + (old_chunk.effect & 0x0F);
          break;

        case fx_ex_ExtendedCmd:
          if ((old_chunk.effect & 0x0F) <= 9) // ef_SetModulatorVol
          {
            chunk.effect_def = ef_Extended;
            chunk.effect = ef_ex_ExtendedCmd2 << 4;

            switch (old_chunk.effect & 0x0F)
            {
              case 0: chunk.effect += ef_ex_cmd2_RSS; break;
              case 1: chunk.effect += ef_ex_cmd2_LockVol; break;
              case 2: chunk.effect += ef_ex_cmd2_UnlockVol; break;
              case 3: chunk.effect += ef_ex_cmd2_LockVP; break;
              case 4: chunk.effect += ef_ex_cmd2_UnlockVP; break;

              case 5:
                chunk.effect_def = processing_whole_song ? 255 : 0;
                chunk.effect = 0;
                adsr_carrier[chan] = true;
                break;

              case 6:
                chunk.effect_def = processing_whole_song ? 255 : 0;
                chunk.effect = processing_whole_song  ? 1 : 0;
                adsr_carrier[chan] = false;
                break;

              case 7: chunk.effect += ef_ex_cmd2_VSlide_car; break;
              case 8: chunk.effect += ef_ex_cmd2_VSlide_mod; break;
              case 9: chunk.effect += ef_ex_cmd2_VSlide_def; break;

              default: break;
            }
          }
          else
          {
            chunk.effect_def = 0;
            chunk.effect = 0;
          }
          break;

        default: break;
      }
      break;

    default: break;
  }

  put_chunk (patt, line, chan + 1, &chunk);
}
