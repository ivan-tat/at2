// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// chan: 1..20
void set_ins_volume (uint8_t modulator, uint8_t carrier, uint8_t chan) {
#if !GO32
  // ** OPL3 emulation workaround **
  // Force muted instrument volume with missing channel ADSR data
  // when there is additionally no FM-reg macro defined for this instrument
  if (is_chan_adsr_data_empty (chan)
     && !songdata.instr_macros[voice_table[chan - 1] - 1].length) {
    modulator = 63;
    carrier = 63;
  }
#endif // !GO32

  if (modulator != BYTE_NULL) {
    uint8_t temp = modulator;

    if ((ins_parameter (voice_table[chan - 1], 10) & 1)
        || (percussion_mode && (17 <= chan) && (chan <= 20))) {
      if (volume_scaling)
        modulator = scale_volume (ins_parameter (voice_table[chan - 1], 2) & 0x3F,
                                  modulator);

      opl3out (_instr[2] + _chan_m[chan - 1],
              scale_volume (scale_volume (modulator,
                                          scale_volume (63 - global_volume,
                                                        63 - fade_out_volume)),
                            63 - overall_volume)
              + (vscale_table[chan - 1] & 0xFF));
    } else
      opl3out (_instr[2] + _chan_m[chan - 1], temp + (vscale_table[chan - 1] & 0xFF));

    volume_table[chan - 1] = concw (temp, (volume_table[chan - 1] >> 8) & 0xFF);

    if ((ins_parameter (voice_table[chan - 1], 10) & 1)
        || (percussion_mode && (17 <= chan) && (chan <= 20)))
      modulator_vol[chan - 1] = 63 - scale_volume (modulator,
                                                   scale_volume (63 - global_volume,
                                                                 63 - fade_out_volume));
    else
      modulator_vol[chan - 1] = 63 - modulator;
  }

  if (carrier != BYTE_NULL) {
    uint8_t temp = carrier;

    if (volume_scaling)
      carrier = scale_volume (ins_parameter (voice_table[chan - 1], 3) & 0x3F,
                              carrier);

    opl3out (_instr[3] + _chan_c[chan - 1],
             scale_volume (scale_volume (carrier,
                                         scale_volume (63 - global_volume,
                                                       63 - fade_out_volume)),
                           63 - overall_volume)
             + ((vscale_table[chan - 1] >> 8) & 0xFF));

    volume_table[chan - 1] = concw (volume_table[chan - 1] & 0xFF, temp);

    carrier_vol[chan - 1] = 63 - scale_volume (carrier,
                                               scale_volume (63 - global_volume,
                                                             63 - fade_out_volume));
  }
}