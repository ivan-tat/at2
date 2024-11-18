// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*static*/ void update_extra_fine_effects (void)
{
  for (uint8_t chan = 0; chan < songdata.nm_tracks; chan++)
  {
    uint8_t eLo  = effect_table[chan] & 0xFF;
    uint8_t eHi  = effect_table[chan] >> 8;
    uint8_t eLo2 = effect_table2[chan] & 0xFF;
    uint8_t eHi2 = effect_table2[chan] >> 8;

    switch (eLo)
    {
      case ef_Extended2 + ef_fix2 + ef_ex2_GlVolSldUpXF:
        global_volume_slide (eHi, BYTE_NULL);
        break;

      case ef_Extended2 + ef_fix2 + ef_ex2_GlVolSldDnXF:
        global_volume_slide (BYTE_NULL, eHi);
        break;

      case ef_Extended2 + ef_fix2 + ef_ex2_VolSlideUpXF:
        volume_slide (chan + 1, eHi, 0);
        break;

      case ef_Extended2 + ef_fix2 + ef_ex2_VolSlideDnXF:
        volume_slide (chan + 1, 0, eHi);
        break;

      case ef_Extended2 + ef_fix2 + ef_ex2_FreqSlideUpXF:
        portamento_up (chan + 1, eHi, nFreq (12*8+1));
        break;

      case ef_Extended2 + ef_fix2 + ef_ex2_FreqSlideDnXF:
        portamento_down (chan + 1, eHi, nFreq (0));
        break;

      case ef_ExtraFineArpeggio:
        arpeggio (chan + 1);
        break;

      case ef_ExtraFineVibrato:
        if (!vibr_table[chan].fine)
          vibrato (chan + 1);
        break;

      case ef_ExtraFineTremolo:
        if (!trem_table[chan].fine)
          tremolo (chan + 1);
        break;

      default:
        break;
    }

    switch (eLo2)
    {
      case ef_Extended2 + ef_fix2 + ef_ex2_GlVolSldUpXF:
        global_volume_slide (eHi2, BYTE_NULL);
        break;

      case ef_Extended2 + ef_fix2 + ef_ex2_GlVolSldDnXF:
        global_volume_slide (BYTE_NULL, eHi2);
        break;

      case ef_Extended2 + ef_fix2 + ef_ex2_VolSlideUpXF:
        volume_slide (chan + 1, eHi2, 0);
        break;

      case ef_Extended2 + ef_fix2 + ef_ex2_VolSlideDnXF:
        volume_slide (chan + 1, 0, eHi2);
        break;

      case ef_Extended2 + ef_fix2 + ef_ex2_FreqSlideUpXF:
        portamento_up (chan + 1, eHi2, nFreq (12*8+1));
        break;

      case ef_Extended2 + ef_fix2 + ef_ex2_FreqSlideDnXF:
        portamento_down (chan + 1, eHi2, nFreq (0));
        break;

      case ef_ExtraFineArpeggio:
        arpeggio2 (chan + 1);
        break;

      case ef_ExtraFineVibrato:
        if (!vibr_table2[chan].fine)
          vibrato2 (chan + 1);
        break;

      case ef_ExtraFineTremolo:
        if (!trem_table2[chan].fine)
          tremolo2 (chan + 1);
        break;

      default:
        break;
    }
  }
}
