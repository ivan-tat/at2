// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
/*static*/ void update_fine_effects (uint8_t chan)
{
  chan--;

  uint8_t eLo  = effect_table[chan] & 0xFF;
  uint8_t eHi  = effect_table[chan] >> 8;
  uint8_t eLo2 = effect_table2[chan] & 0xFF;
  uint8_t eHi2 = effect_table2[chan] >> 8;

  switch (eLo)
  {
    case ef_ArpggVSlideFine:
      volume_slide (chan + 1, eHi / 16, eHi % 16);
      break;

    case ef_FSlideUpFine:
      portamento_up (chan + 1, eHi, nFreq (12*8+1));
      break;

    case ef_FSlideDownFine:
      portamento_down (chan + 1, eHi, nFreq (0));
      break;

    case ef_FSlUpVSlF:
      volume_slide (chan + 1, eHi / 16, eHi % 16);
      break;

    case ef_FSlDownVSlF:
      volume_slide (chan + 1, eHi / 16, eHi % 16);
      break;

    case ef_FSlUpFineVSlide:
      portamento_up (chan + 1, fslide_table[chan], nFreq (12*8+1));
      break;

    case ef_FSlUpFineVSlF:
      portamento_up (chan + 1, fslide_table[chan], nFreq (12*8+1));
      volume_slide (chan + 1, eHi / 16, eHi % 16);
      break;

    case ef_FSlDownFineVSlide:
      portamento_down (chan + 1, fslide_table[chan], nFreq (0));
      break;

    case ef_FSlDownFineVSlF:
      portamento_down (chan + 1, fslide_table[chan], nFreq (0));
      volume_slide (chan + 1, eHi / 16, eHi % 16);
      break;

    case ef_TPortamVSlideFine:
      volume_slide (chan + 1, eHi / 16, eHi % 16);
      break;

    case ef_Vibrato:
      if (vibr_table[chan].fine)
        vibrato (chan + 1);
      break;

    case ef_Tremolo:
      if (trem_table[chan].fine)
        tremolo (chan + 1);
      break;

    case ef_VibratoVolSlide:
      if (vibr_table[chan].fine)
        vibrato (chan + 1);
      break;

    case ef_VibratoVSlideFine:
      volume_slide (chan + 1, eHi / 16, eHi % 16);
      if (vibr_table[chan].fine)
        vibrato (chan + 1);
      break;

    case ef_VolSlideFine:
      volume_slide (chan + 1, eHi / 16, eHi % 16);
      break;

    case ef_Extended2 + ef_fix2 + ef_ex2_GlVolSlideUpF:
      global_volume_slide (eHi, BYTE_NULL);
      break;

    case ef_Extended2 + ef_fix2 + ef_ex2_GlVolSlideDnF:
      global_volume_slide (BYTE_NULL, eHi);
      break;

    default:
      break;
  }

  switch (eLo2)
  {
    case ef_ArpggVSlideFine:
      volume_slide (chan + 1, eHi2 / 16, eHi2 % 16);
      break;

    case ef_FSlideUpFine:
      portamento_up (chan + 1, eHi2, nFreq (12*8+1));
      break;

    case ef_FSlideDownFine:
      portamento_down (chan + 1, eHi2, nFreq (0));
      break;

    case ef_FSlUpVSlF:
      volume_slide (chan + 1, eHi2 / 16, eHi2 % 16);
      break;

    case ef_FSlDownVSlF:
      volume_slide (chan + 1, eHi2 / 16, eHi2 % 16);
      break;

    case ef_FSlUpFineVSlide:
      portamento_up (chan + 1, fslide_table2[chan], nFreq (12*8+1));
      break;

    case ef_FSlUpFineVSlF:
      portamento_up (chan + 1, fslide_table2[chan], nFreq (12*8+1));
      volume_slide (chan + 1, eHi2 / 16, eHi2 % 16);
      break;

    case ef_FSlDownFineVSlide:
      portamento_down (chan + 1, fslide_table2[chan], nFreq (0));
      break;

    case ef_FSlDownFineVSlF:
      portamento_down (chan + 1, fslide_table2[chan], nFreq (0));
      volume_slide (chan + 1, eHi2 / 16, eHi2 % 16);
      break;

    case ef_TPortamVSlideFine:
      volume_slide (chan + 1, eHi2 / 16, eHi2 % 16);
      break;

    case ef_Vibrato:
      if (vibr_table2[chan].fine)
        vibrato2 (chan + 1);
      break;

    case ef_Tremolo:
      if (trem_table2[chan].fine)
        tremolo2 (chan + 1);
      break;

    case ef_VibratoVolSlide:
      if (vibr_table2[chan].fine)
        vibrato2 (chan + 1);
      break;

    case ef_VibratoVSlideFine:
      volume_slide (chan + 1, eHi2 / 16, eHi2 % 16);
      if (vibr_table2[chan].fine)
        vibrato2 (chan + 1);
      break;

    case ef_VolSlideFine:
      volume_slide (chan + 1, eHi2 / 16, eHi2 % 16);
      break;

    case ef_Extended2 + ef_fix2 + ef_ex2_GlVolSlideUpF:
      global_volume_slide (eHi2, BYTE_NULL);
      break;

    case ef_Extended2 + ef_fix2 + ef_ex2_GlVolSlideDnF:
      global_volume_slide (BYTE_NULL, eHi2);
      break;

    default:
      break;
  }
}
