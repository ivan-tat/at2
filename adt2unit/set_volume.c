// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
static void set_volume (uint8_t modulator, uint8_t carrier, uint8_t chan)
{
  chan--;

#if !GO32
  // OPL3 EMULATION WORKAROUND
  // Force muted instrument volume with missing channel ADSR data
  // when there is additionally no FM-reg macro defined for this instrument
  if (is_chan_adsr_data_empty (chan + 1)
      && !songdata.instr_macros[voice_table[chan] - 1].length)
  {
    modulator = 63;
    carrier = 63;
  }
#endif // !GO32

  if (modulator != BYTE_NULL)
  {
    uint8_t temp = modulator;
    uint8_t vol;

    modulator = scale_volume (ins_parameter (voice_table[chan], 2) & 0x3F,
                              modulator);
    vol = scale_volume (modulator, scale_volume (63 - global_volume,
                                                 63 - fade_out_volume));

    opl3out (_instr[2] + _chan_m[chan],
             scale_volume (vol, 63 - overall_volume)
             + (vscale_table[chan] & 0xFF));

    volume_table[chan] = temp + (volume_table[chan] & 0xFF00);
    modulator_vol[chan] = 63 - vol;
  }

  if (carrier != BYTE_NULL)
  {
    uint8_t temp = carrier;
    uint8_t vol;

    carrier = scale_volume (ins_parameter (voice_table[chan], 3) & 0x3F,
                            carrier);
    vol = scale_volume (carrier, scale_volume (63 - global_volume,
                                               63 - fade_out_volume));

    opl3out (_instr[3] + _chan_c[chan],
             scale_volume (vol, 63 - overall_volume)
             + ((vscale_table[chan] >> 8) & 0xFF));

    volume_table[chan] = (volume_table[chan] & 0x00FF) + (temp << 8);
    carrier_vol[chan] = 63 - vol;
  }
}
