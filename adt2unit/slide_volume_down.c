// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
/*static*/ void slide_volume_down (uint8_t chan, uint8_t slide)
{
  chan--;

  switch (volslide_type[chan])
  {
    case 0:
      if (!_4op_vol_valid_chan (chan + 1))
      {
        slide_carrier_volume_down (chan, slide);
        if ((ins_parameter (voice_table[chan], 10) & 1)
            || (percussion_mode && (chan >= 16) && (chan <= 19)))
          slide_modulator_volume_down (chan, slide);
      }
      else
      {
        uint32_t _4op_flag = _4op_data_flag (chan + 1);
        uint8_t  _4op_conn =  (_4op_flag >> 1) & 3; // 0..3
        int8_t   _4op_ch1  = ((_4op_flag >> 3) & 15) - 1;
        int8_t   _4op_ch2  = ((_4op_flag >> 7) & 15) - 1;

        switch (_4op_conn)
        {
          case 0: // FM/FM
            slide_carrier_volume_down (_4op_ch1, slide);
            break;

          case 1: // FM/AM
            slide_carrier_volume_down (_4op_ch1, slide);
            slide_modulator_volume_down (_4op_ch2, slide);
            break;

          case 2: // AM/FM
            slide_carrier_volume_down (_4op_ch1, slide);
            slide_carrier_volume_down (_4op_ch2, slide);
            break;

          case 3: // AM/AM
            slide_carrier_volume_down (_4op_ch1, slide);
            slide_modulator_volume_down (_4op_ch1, slide);
            slide_modulator_volume_down (_4op_ch2, slide);
            break;
        }
      }
      break;

    case 1:
      slide_carrier_volume_down (chan, slide);
      break;

    case 2:
      slide_modulator_volume_down (chan, slide);
      break;

    case 3:
      slide_carrier_volume_down (chan, slide);
      slide_modulator_volume_down (chan, slide);
      break;

    default: // FIXME: never reached?
      break;
  }
}
