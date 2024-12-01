// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void decay_bars_refresh (void)
{
  double speedup_factor, speedown_factor;
  double rise_coef, fall_coef;
  uint8_t attrs[2]; // output screen attributes
  uint8_t chan;

  DBG_ENTER ("decay_bars_refresh");

  show_bar (0 + 1, overall_volume, 5, 1, 2,
            analyzer_ovrllvol + (analyzer_ovrllvol << 4));
  show_bar (0 + 1, global_volume, MAX_COLUMNS - 7, 1, 2,
            analyzer_volumelvl + (analyzer_volumelvl << 4));

#if GO32
  speedup_factor = 4.0 * (fps_down_factor + 1);
#else // !GO32
  speedup_factor = 2.5 * (100 / sdl_frame_rate);
#endif // !GO32
  speedown_factor = speedup_factor;
  rise_coef = decay_bar_rise * speedup_factor;
  fall_coef = decay_bar_fall * speedown_factor;

  attrs[0] = analyzer + (analyzer_modulator << 4);
  attrs[1] = analyzer + (analyzer_carrier << 4);
  chan = chan_pos - 1;
  for (uint8_t i = 0; i < MAX_TRACKS; i++)
  {
    bool perc = percussion_mode && (chan >= 16) && (chan <= 19);
    uint8_t levels[2];

    decay_bar[chan].lvl1 += decay_bar[chan].dir1
                            * ((decay_bar[chan].dir1 == 1) ? rise_coef : fall_coef);

    if (decay_bar[chan].lvl1 < 0) decay_bar[chan].lvl1 = 0;
    else if (decay_bar[chan].lvl1 > 63) decay_bar[chan].lvl1 = 63;

    if (decay_bar[chan].lvl1 >= carrier_vol[chan]) decay_bar[chan].dir1 = -1;

    decay_bar[chan].lvl2 += decay_bar[chan].dir2
                            * ((decay_bar[chan].dir2 == 1) ? rise_coef : fall_coef);

    if (decay_bar[chan].lvl2 < 0) decay_bar[chan].lvl2 = 0;
    else if (decay_bar[chan].lvl2 > 63) decay_bar[chan].lvl2 = 63;

    if (decay_bar[chan].lvl2 >= modulator_vol[chan]) decay_bar[chan].dir2 = -1;

    volum_bar[chan].lvl += volum_bar[chan].dir
                           * ((volum_bar[chan].dir == 1) ? rise_coef : fall_coef);

    if (volum_bar[chan].lvl < 0) volum_bar[chan].lvl = 0;
    else if (volum_bar[chan].lvl > 63) volum_bar[chan].lvl = 63;

    if (!perc)
    {
      if (ins_parameter (voice_table[chan], 10) & 1)
      {
        if (volum_bar[chan].lvl >= ((carrier_vol[chan] + modulator_vol[chan]) / 2))
          volum_bar[chan].dir = -1;
      }
      else if (volum_bar[chan].lvl >= carrier_vol[chan])
        volum_bar[chan].dir = -1;
    }
    else if (volum_bar[chan].lvl >= modulator_vol[chan])
      volum_bar[chan].dir = -1;

    if ((ins_parameter (voice_table[chan], 10) & 1) || perc)
      levels[0] = decay_bar[chan].lvl2;
    else
      levels[0] = 63 - (scale_volume (63 - decay_bar[chan].lvl2,
                                      63 - decay_bar[chan].lvl1));
    if (!perc)
      levels[1] = decay_bar[chan].lvl1;
    else
      levels[1] = 0;

    show_bar (i + 1, levels[0], 8, 1, 6, attrs[0]);
    show_bar (i + 1, levels[1], 8, 8, 14, attrs[1]);

    chan++;
  }

  DBG_LEAVE (); //EXIT //decay_bars_refresh
}
