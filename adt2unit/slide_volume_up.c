// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
/*static*/ void slide_volume_up (uint8_t chan, uint8_t slide)
{
  chan--;

  bool     flag = _4op_vol_valid_chan (chan + 1);
  uint32_t _4op_flag = _4op_data_flag (chan + 1);
  int8_t   _4op_ch1  = ((_4op_flag >>  3) & 15) - 1;
  int8_t   _4op_ch2  = ((_4op_flag >>  7) & 15) - 1;
  uint8_t  limit1, limit2;
  uint16_t limit1_4op, limit2_4op;

  if (flag)
  {
    uint8_t _4op_ins1 = (_4op_flag >> 11) & 0xFF;
    uint8_t _4op_ins2 = (_4op_flag >> 19) & 0xFF;

    limit1 = 0;
    limit2 = 0;
    limit1_4op = peak_lock[_4op_ch1] ?  ((ins_parameter (_4op_ins1, 3) & 0x3F) << 8) // FIXME: was `<< 16'
                                       + (ins_parameter (_4op_ins1, 2) & 0x3F)
                                     : 0;
    limit2_4op = peak_lock[_4op_ch2] ?  ((ins_parameter (_4op_ins2, 3) & 0x3F) << 8) // FIXME: was `<< 16'
                                       + (ins_parameter (_4op_ins2, 2) & 0x3F)
                                     : 0;
  }
  else
  {
    limit1 = peak_lock[chan] ? ins_parameter (event_table[chan].instr_def, 3) & 0x3F
                             : 0;
    limit2 = peak_lock[chan] ? ins_parameter (event_table[chan].instr_def, 2) & 0x3F
                             : 0;
    limit1_4op = 0;
    limit2_4op = 0;
  }

  switch (volslide_type[chan])
  {
    case 0:
      if (!flag)
      {
        slide_carrier_volume_up (chan, slide, limit1);
        if ((ins_parameter (voice_table[chan], 10) & 1)
            || (percussion_mode && (chan >= 16) && (chan <= 19)))
          slide_modulator_volume_up (chan, slide, limit2);
      }
      else
      {
        uint8_t _4op_conn = (_4op_flag >> 1) & 3; // 0..3

        switch (_4op_conn)
        {
          case 0: // FM/FM
            slide_carrier_volume_up (_4op_ch1, slide, limit1_4op >> 8);
            break;

          case 1: // FM/AM
            slide_carrier_volume_up (_4op_ch1, slide, limit1_4op >> 8);
            slide_modulator_volume_up (_4op_ch2, slide, limit2_4op & 0xFF);
            break;

          case 2: // AM/FM
            slide_carrier_volume_up (_4op_ch1, slide, limit1_4op >> 8);
            slide_carrier_volume_up (_4op_ch2, slide, limit2_4op >> 8);
            break;

          case 3: // AM/AM
            slide_carrier_volume_up (_4op_ch1, slide, limit1_4op >> 8);
            slide_modulator_volume_up (_4op_ch1, slide, limit1_4op & 0xFF);
            slide_modulator_volume_up (_4op_ch2, slide, limit2_4op & 0xFF);
            break;
       }
      }
      break;

    case 1:
      slide_carrier_volume_up (chan, slide, limit1);
      break;

    case 2:
      slide_modulator_volume_up (chan, slide, limit2);
      break;

    case 3:
      slide_carrier_volume_up (chan, slide, limit1);
      slide_modulator_volume_up (chan, slide, limit2);
      break;

    default: // FIXME: never reached?
      break;
  }
}
