// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_pat_event_a2m_v1 (tCHUNK *dst, pat_event_a2m_v1_t *src,
                                  bool *adsr_carrier, bool processing_whole_song)
{
  memset (dst, 0, sizeof (*dst));
  dst->note       = src->note;
  dst->instr_def  = src->ins;
  dst->effect_def = src->cmd;
  dst->effect     = src->data;

  switch (src->cmd)
  {
    case 0: dst->effect_def = ef_Arpeggio; break;         // Arpeggio
    case 1: dst->effect_def = ef_FSlideUp; break;         // Frequency Slide Up
    case 2: dst->effect_def = ef_FSlideDown; break;       // Frequency Slide Down
    case 3: dst->effect_def = ef_FSlideUpFine; break;     // Fine Frequency Slide Up
    case 4: dst->effect_def = ef_FSlideDownFine; break;   // Fine Frequency Slide Down
    case 5: dst->effect_def = ef_TonePortamento; break;   // Tone Portamento
    case 6: dst->effect_def = ef_TPortamVolSlide; break;  // Tone Portamento & Volume Slide
    case 7: dst->effect_def = ef_Vibrato; break;          // Vibrato
    case 8: dst->effect_def = ef_VibratoVolSlide; break;  // Vibrato & Volume Slide

    case 9: // Set Operator Intensity
      if ((src->data & 0xF0) != 0)
      {
        dst->effect_def = ef_SetCarrierVol;
        dst->effect     = ((src->data & 0xF0) >> 2) + 3;
      }
      else if ((src->data & 0x0F) != 0)
      {
        dst->effect_def = ef_SetModulatorVol;
        dst->effect     = ((src->data & 0x0F) << 2) + 3;
      }
      else
        dst->effect_def = 0;
      break;

    case 10: dst->effect_def = ef_SetInsVolume; break;  // Set Instrument Volume
    case 11: dst->effect_def = ef_PatternBreak; break;  // Pattern Break
    case 12: dst->effect_def = ef_PositionJump; break;  // Pattern Jump
    case 13: dst->effect_def = ef_SetSpeed; break;      // Set Tempo
    case 14: dst->effect_def = ef_SetTempo; break;      // Set Timer

    case 15: // Extended
      switch (src->data >> 4)
      {
        case 0: // Extended: Default AM Depth
          dst->effect_def = ef_Extended;
          dst->effect     = (ef_ex_SetTremDepth << 4) + (src->data & 0x0F);
          break;

        case 1: // Extended: Default Vibrato Depth
          dst->effect_def = ef_Extended;
          dst->effect     = (ef_ex_SetVibDepth << 4) + (src->data & 0x0F);
          break;

        case 2: // Extended: Default Waveform
          dst->effect_def = ef_SetWaveform;
          switch (src->data & 0x0F)
          {
            case 0: case 1: case 2: case 3: dst->effect = ((src->data & 0x0F) << 4) + 0x0F; break;
            case 4: case 5: case 6: case 7: dst->effect = 0xF0 + (src->data & 0x0F) - 4; break;
            default: break;
          }
          break;

        case 3: // Extended: Manual Frequency Slide Up
          dst->effect_def = ef_Extended2;
          dst->effect     = (ef_ex2_FineTuneUp << 4) + (src->data & 0x0F);
          break;

        case 4: // Extended: Manual Frequency Slide Down
          dst->effect_def = ef_Extended2;
          dst->effect     = (ef_ex2_FineTuneDown << 4) + (src->data & 0x0F);
          break;

        case 5: // Extended: Volume Slide Up
          dst->effect_def = ef_VolSlide;
          dst->effect     = (src->data & 0x0F) << 4;
          break;

        case 6: // Extended: Volume Slide Down
          dst->effect_def = ef_VolSlide;
          dst->effect     = src->data & 0x0F;
          break;

        case 7: // Extended: Fine Volume Slide Up
          dst->effect_def = ef_VolSlideFine;
          dst->effect     = (src->data & 0x0F) << 4;
          break;

        case 8: // Extended: Fine Volume Slide Down
          dst->effect_def = ef_VolSlideFine;
          dst->effect     = src->data & 0x0F;
          break;

        case 9: // Extended: Retrigger Note
          dst->effect_def = ef_RetrigNote;
          dst->effect     = (src->data & 0x0F) + 1;
          break;

        case 10:  // Extended: Set Attack Rate
          dst->effect_def = ef_Extended;
          dst->effect     = ((*adsr_carrier ? ef_ex_SetAttckRateC : ef_ex_SetAttckRateM) << 4) + (src->data & 0x0F);
          break;

        case 11:  // Extended: Set Decay Rate
          dst->effect_def = ef_Extended;
          dst->effect     = ((*adsr_carrier ? ef_ex_SetDecayRateC : ef_ex_SetDecayRateM) << 4) + (src->data & 0x0F);
          break;

        case 12:  // Extended: Set Sustain Level
          dst->effect_def = ef_Extended;
          dst->effect     = ((*adsr_carrier ? ef_ex_SetSustnLevelC : ef_ex_SetSustnLevelM) << 4) + (src->data & 0x0F);
          break;

        case 13:  // Extended: Set Release Rate
          dst->effect_def = ef_Extended;
          dst->effect     = ((*adsr_carrier ? ef_ex_SetRelRateC : ef_ex_SetRelRateM) << 4) + (src->data & 0x0F);
          break;

        case 14:  // Extended: Set Feedback
          dst->effect_def = ef_Extended;
          dst->effect     = (ef_ex_SetFeedback << 4) + (src->data & 0x0F);
          break;

        case 15:  // Extended: Extended Command
          if ((src->data & 0x0F) <= 9) // ef_SetModulatorVol
          {
            dst->effect_def = ef_Extended;
            dst->effect     = ef_ex_ExtendedCmd2 << 4;

            switch (src->data & 0x0F)
            {
              case 0: dst->effect += ef_ex_cmd2_RSS; break;
              case 1: dst->effect += ef_ex_cmd2_LockVol; break;
              case 2: dst->effect += ef_ex_cmd2_UnlockVol; break;
              case 3: dst->effect += ef_ex_cmd2_LockVP; break;
              case 4: dst->effect += ef_ex_cmd2_UnlockVP; break;

              case 5:
                dst->effect_def = processing_whole_song ? 255 : 0;
                dst->effect     = 0;
                *adsr_carrier = true;
                break;

              case 6:
                dst->effect_def = processing_whole_song ? 255 : 0;
                dst->effect     = processing_whole_song ? 1 : 0;
                *adsr_carrier = false;
                break;

              case 7: dst->effect += ef_ex_cmd2_VSlide_car; break;
              case 8: dst->effect += ef_ex_cmd2_VSlide_mod; break;
              case 9: dst->effect += ef_ex_cmd2_VSlide_def; break;

              default: break;
            }
          }
          else
          {
            dst->effect_def = 0;
            dst->effect     = 0;
          }
          break;

        default: break;
      }
      break;

    default: break;
  }
}
