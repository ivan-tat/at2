// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
static uint8_t chanvol (uint8_t chan)
{
  chan--;

  uint16_t x = volume_table[chan] >> 8;

  if (ins_parameter (voice_table[chan], 10) & 1)
    x = (x + (volume_table[chan] & 0xFF) + 1) / 2;

  return 63 - x;
}

/*static*/ void update_effects (void)
{
  for (uint8_t chan = 0; chan < songdata.nm_tracks; chan++)
  {
    uint8_t eLo = effect_table[chan] & 0xFF;
    uint8_t eHi = effect_table[chan] >> 8;
    uint8_t eLo2 = effect_table2[chan] & 0xFF;
    uint8_t eHi2 = effect_table2[chan] >> 8;

    switch (eLo)
    {
      case ef_Arpeggio + ef_fix1:
        arpeggio (chan + 1);
        break;

      case ef_ArpggVSlide:
        volume_slide (chan + 1, eHi / 16, eHi % 16);
        arpeggio (chan + 1);
        break;

      case ef_ArpggVSlideFine:
        arpeggio (chan + 1);
        break;

      case ef_FSlideUp:
        portamento_up (chan + 1, eHi, nFreq (12*8+1));
        break;

      case ef_FSlideDown:
        portamento_down (chan + 1, eHi, nFreq (0));
        break;

      case ef_FSlideUpVSlide:
        portamento_up (chan + 1, fslide_table[chan], nFreq (12*8+1));
        volume_slide (chan + 1, eHi / 16, eHi % 16);
        break;

      case ef_FSlUpVSlF:
        portamento_up (chan + 1, fslide_table[chan], nFreq (12*8+1));
        break;

      case ef_FSlideDownVSlide:
        portamento_down (chan + 1, fslide_table[chan], nFreq (0));
        volume_slide (chan + 1, eHi / 16, eHi % 16);
        break;

      case ef_FSlDownVSlF:
        portamento_down (chan + 1, fslide_table[chan], nFreq (0));
        break;

      case ef_FSlUpFineVSlide:
        volume_slide (chan + 1, eHi / 16, eHi % 16);
        break;

      case ef_FSlDownFineVSlide:
        volume_slide (chan + 1, eHi / 16, eHi % 16);
        break;

      case ef_TonePortamento:
        tone_portamento (chan + 1);
        break;

      case ef_TPortamVolSlide:
        volume_slide (chan + 1, eHi / 16, eHi % 16);
        tone_portamento (chan + 1);
        break;

      case ef_TPortamVSlideFine:
        tone_portamento (chan + 1);
        break;

      case ef_Vibrato:
        if (!vibr_table[chan].fine)
          vibrato (chan + 1);
        break;

      case ef_Tremolo:
        if (!trem_table[chan].fine)
          tremolo (chan + 1);
        break;

      case ef_VibratoVolSlide:
        volume_slide (chan + 1, eHi / 16, eHi % 16);
        if (!vibr_table[chan].fine)
          vibrato (chan + 1);
        break;

      case ef_VibratoVSlideFine:
        if (!vibr_table[chan].fine)
          vibrato (chan + 1);
        break;

      case ef_VolSlide:
        volume_slide (chan + 1, eHi / 16, eHi % 16);
        break;

      case ef_RetrigNote:
        if (retrig_table[chan] >= eHi)
        {
          retrig_table[chan] = 0;
          output_note (event_table[chan].note,
                       event_table[chan].instr_def, chan + 1, true, true);
        }
        else
          retrig_table[chan]++;
        break;

      case ef_MultiRetrigNote:
        if (retrig_table[chan] >= (eHi / 16))
        {
          switch (eHi % 16)
          {
            case 0: break;
            case 1: slide_volume_down (chan + 1, 1); break;
            case 2: slide_volume_down (chan + 1, 2); break;
            case 3: slide_volume_down (chan + 1, 4); break;
            case 4: slide_volume_down (chan + 1, 8); break;
            case 5: slide_volume_down (chan + 1, 16); break;

            case 6:
              {
                uint8_t x = chanvol (chan + 1);
                slide_volume_down (chan + 1, x - ((x * 2 + 1) / 3));
              }
              break;

            case 7:
              {
                uint8_t x = chanvol (chan + 1);
                slide_volume_down (chan + 1, x - ((x + 1) / 2));
              }
              break;

            case 8: break;
            case 9: slide_volume_up (chan + 1, 1); break;
            case 10: slide_volume_up (chan + 1, 2); break;
            case 11: slide_volume_up (chan + 1, 4); break;
            case 12: slide_volume_up (chan + 1, 8); break;
            case 13: slide_volume_up (chan + 1, 16); break;

            case 14:
              {
                uint8_t x = chanvol (chan + 1);
                slide_volume_up (chan + 1, max (((x * 3 + 1) / 2) - x, 63));
              }
              break;

            case 15:
              {
                uint8_t x = chanvol (chan + 1);
                slide_volume_up (chan + 1, max (x * 2 - x, 63));
              }
              break;
          }

          retrig_table[chan] = 0;
          output_note (event_table[chan].note,
                       event_table[chan].instr_def, chan + 1, true, true);
        }
        else
          retrig_table[chan]++;
        break;

      case ef_Tremor:
        if (tremor_table[chan].pos >= 0)
        {
          if ((tremor_table[chan].pos + 1) <= (eHi / 16))
            tremor_table[chan].pos++;
          else
          {
            slide_volume_down (chan + 1, 63);
            tremor_table[chan].pos = -1;
          }
        }
        else
          if ((tremor_table[chan].pos - 1) >= -(eHi % 16))
            tremor_table[chan].pos--;
          else
          {
            set_ins_volume (tremor_table[chan].volume & 0xFF,
                            tremor_table[chan].volume >> 8, chan + 1);
            tremor_table[chan].pos = 1;
          }
        break;

      case ef_Extended2 + ef_fix2 + ef_ex2_NoteDelay:
        if (!notedel_table[chan])
        {
          notedel_table[chan] = BYTE_NULL;
          output_note (event_table[chan].note,
                       event_table[chan].instr_def, chan + 1, true, true);
        }
        else
          notedel_table[chan]--;
        break;

      case ef_Extended2 + ef_fix2 + ef_ex2_NoteCut:
        if (!notecut_table[chan])
        {
          notecut_table[chan] = BYTE_NULL;
          key_off (chan + 1);
        }
        else
          notecut_table[chan]--;
        break;

      case ef_Extended2 + ef_fix2 + ef_ex2_GlVolSlideUp:
        global_volume_slide (eHi, BYTE_NULL);
        break;

      case ef_Extended2 + ef_fix2 + ef_ex2_GlVolSlideDn:
        global_volume_slide (BYTE_NULL, eHi);
        break;

      default:
        break;
    }

    switch (eLo2)
    {
      case ef_Arpeggio + ef_fix1:
        arpeggio2 (chan + 1);
        break;

      case ef_ArpggVSlide:
        volume_slide (chan + 1, eHi2 / 16, eHi2 % 16);
        arpeggio2 (chan + 1);
        break;

      case ef_ArpggVSlideFine:
        arpeggio2 (chan + 1);
        break;

      case ef_FSlideUp:
        portamento_up (chan + 1, eHi2, nFreq (12*8+1));
        break;

      case ef_FSlideDown:
        portamento_down (chan + 1, eHi2, nFreq (0));
        break;

      case ef_FSlideUpVSlide:
        portamento_up (chan + 1, fslide_table2[chan], nFreq (12*8+1));
        volume_slide (chan + 1, eHi2 / 16, eHi2 % 16);
        break;

      case ef_FSlUpVSlF:
        portamento_up (chan + 1, fslide_table2[chan], nFreq (12*8+1));
        break;

      case ef_FSlideDownVSlide:
        portamento_down (chan + 1, fslide_table2[chan], nFreq (0));
        volume_slide (chan + 1, eHi2 / 16, eHi2 % 16);
        break;

      case ef_FSlDownVSlF:
        portamento_down (chan + 1, fslide_table2[chan], nFreq (0));
        break;

      case ef_FSlUpFineVSlide:
        volume_slide (chan + 1, eHi2 / 16, eHi2 % 16);
        break;

      case ef_FSlDownFineVSlide:
        volume_slide (chan + 1, eHi2 / 16, eHi2 % 16);
        break;

      case ef_TonePortamento:
        tone_portamento2 (chan + 1);
        break;

      case ef_TPortamVolSlide:
        volume_slide (chan + 1, eHi2 / 16, eHi2 % 16);
        tone_portamento2 (chan + 1);
        break;

      case ef_TPortamVSlideFine:
        tone_portamento2 (chan + 1);
        break;

      case ef_Vibrato:
        if (!vibr_table2[chan].fine)
          vibrato2 (chan + 1);
        break;

      case ef_Tremolo:
        if (!trem_table2[chan].fine)
          tremolo2 (chan + 1);
        break;

      case ef_VibratoVolSlide:
        volume_slide (chan + 1, eHi2 / 16, eHi2 % 16);
        if (!vibr_table2[chan].fine)
          vibrato2 (chan + 1);
        break;

      case ef_VibratoVSlideFine:
        if (!vibr_table2[chan].fine)
          vibrato2 (chan + 1);
        break;

      case ef_VolSlide:
        volume_slide (chan + 1, eHi2 / 16, eHi2 % 16);
        break;

      case ef_RetrigNote:
        if (retrig_table2[chan] >= eHi2)
        {
          retrig_table2[chan] = 0;
          output_note (event_table[chan].note,
                       event_table[chan].instr_def, chan + 1, true, true);
        }
        else
          retrig_table2[chan]++;
        break;

      case ef_MultiRetrigNote:
        if (retrig_table2[chan] >= (eHi2 / 16))
        {
          switch (eHi2 % 16)
          {
            case 0: break;
            case 1: slide_volume_down (chan + 1, 1); break;
            case 2: slide_volume_down (chan + 1, 2); break;
            case 3: slide_volume_down (chan + 1, 4); break;
            case 4: slide_volume_down (chan + 1, 8); break;
            case 5: slide_volume_down (chan + 1, 16); break;

            case 6:
              {
                uint8_t x = chanvol (chan + 1);
                slide_volume_down (chan + 1, x - ((x * 2 + 1) / 3));
              }
              break;

            case 7:
              {
                uint8_t x = chanvol (chan + 1);
                slide_volume_down (chan + 1, x - ((x + 1) / 2));
              }
              break;

            case 8: break;
            case 9: slide_volume_up (chan + 1, 1); break;
            case 10: slide_volume_up (chan + 1, 2); break;
            case 11: slide_volume_up (chan + 1, 4); break;
            case 12: slide_volume_up (chan + 1, 8); break;
            case 13: slide_volume_up (chan + 1, 16); break;

            case 14:
              {
                uint8_t x = chanvol (chan + 1);
                slide_volume_up (chan + 1, max (((x * 3 + 1) / 2) - x, 63));
              }
              break;

            case 15:
              {
                uint8_t x = chanvol (chan + 1);
                slide_volume_up (chan + 1, max ((x * 2) - x, 63));
              }
              break;
          }

          retrig_table2[chan] = 0;
          output_note (event_table[chan].note,
                       event_table[chan].instr_def, chan + 1, true, true);
        }
        else
          retrig_table2[chan]++;
        break;

      case ef_Tremor:
        if (tremor_table2[chan].pos >= 0)
        {
          if ((tremor_table2[chan].pos + 1) <= (eHi2 / 16))
            tremor_table2[chan].pos++;
          else
          {
            slide_volume_down (chan + 1, 63);
            tremor_table2[chan].pos = -1;
          }
        }
        else
          if ((tremor_table2[chan].pos - 1) >= -(eHi2 % 16))
            tremor_table2[chan].pos--;
          else
          {
            set_ins_volume (tremor_table2[chan].volume & 0xFF,
                            tremor_table2[chan].volume >> 8, chan + 1);
            tremor_table2[chan].pos = 1;
          }
        break;

      case ef_Extended2 + ef_fix2 + ef_ex2_NoteDelay:
        if (!notedel_table[chan])
        {
          notedel_table[chan] = BYTE_NULL;
          output_note (event_table[chan].note,
                       event_table[chan].instr_def, chan + 1, true, true);
        }
        else
          notedel_table[chan]--;
        break;

      case ef_Extended2 + ef_fix2 + ef_ex2_NoteCut:
        if (!notecut_table[chan])
        {
          notecut_table[chan] = BYTE_NULL;
          key_off (chan + 1);
        }
        else
          notecut_table[chan]--;
        break;

      case ef_Extended2 + ef_fix2 + ef_ex2_GlVolSlideUp:
        global_volume_slide (eHi2, BYTE_NULL);
        break;

      case ef_Extended2 + ef_fix2 + ef_ex2_GlVolSlideDn:
        global_volume_slide (BYTE_NULL, eHi2);
        break;

      default:
        break;
    }
  }
}
