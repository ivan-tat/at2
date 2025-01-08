// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*** DECAY BARS window ***/

struct decay_bars_window_t decay_bars_window =
{
  .area =
  {
    .top_left = { .x = 1, .y = 0 },
    .bottom_right = { .x = 0, .y = 0 }
  },
  .track_width = DECAY_BARS_WINDOW__TRACK_WIDTH_MAX,
  .bar_height = DECAY_BARS_WINDOW__BAR_HEIGHT_MAX,
  .levels = { { 0, }, }
};

// trk: 0..19
// level: 0..63
static void decay_bars_window__draw_bar (
  struct decay_bars_window_t *self, uint8_t trk, uint8_t level,
  uint8_t xpos, uint8_t from_pos, uint8_t to_pos, uint8_t attr)
{
  String r[DECAY_BARS_WINDOW__BAR_HEIGHT_MAX+2+1], *s = GetStr (r); // final string
  uint_least8_t i; // last position in the string
  uint8_t x0, x1, y; // output screen coordinates
  uint8_t attrs[2]; // output screen attributes

  if (level)
  {
    uint_least8_t full; // number of full-filled characters (0 = none)
    uint_least8_t tail; // character for tail (0 = none)

    // scale `level' to fit in `self->bar_height' characters
    // (`charmap.vol_bar_up_steps' lines each, `+ 31' for rounding)
    level = (uint16_t)(level
            * self->bar_height
            * charmap.vol_bar_up_steps + 31) / 63;

    full = level / charmap.vol_bar_up_steps; // 0..self->bar_height
    tail = level % charmap.vol_bar_up_steps;

    i = self->bar_height + 2; // +2 characters for color switching
    SetLength (r, i);

    i--;
    s[i] = '~'; // switch color

    while (full)
    {
      i--;
      s[i] = charmap.vol_bar_up_full;
      full--;
    }

    if (tail)
    {
      i--;
      s[i] = charmap.vol_bar_up_tail[tail - 1];
    }

    i--;
    s[i] = '~'; // switch color
  }
  else
  {
    i = 7;
    SetLength (r, i);
  }

  while (i)
  {
    i--;
    s[i] = charmap.vol_bar_up_back;
  }

  x0 = xpos + trk * self->track_width + from_pos - 1;
  x1 = xpos + trk * self->track_width + to_pos - 1;
  y = self->area.top_left.y + 1;
  attrs[0] = analyzer_bckg + analyzer;
  attrs[1] = analyzer_bckg + (attr >> 4);

  for (uint8_t x = x0; x <= x1; x++)
    show_vcstr (x, y, r, attrs[0], attrs[1]);
}

static void decay_bars_window__draw_overall_volume_bar (
  struct decay_bars_window_t *self)
{
  decay_bars_window__draw_bar (self, 0, overall_volume,
    self->area.top_left.x + 4, 1, 2,
    analyzer_ovrllvol + (analyzer_ovrllvol << 4));
}

static void decay_bars_window__draw_global_volume_bar (
  struct decay_bars_window_t *self)
{
  decay_bars_window__draw_bar (self, 0, global_volume,
    self->area.bottom_right.x - 7, 1, 2,
    analyzer_volumelvl + (analyzer_volumelvl << 4));
}

static void decay_bars_window__update_decay_bar_levels (
  struct decay_bars_window_t *self)
{
  double speedup_factor, speedown_factor;
  double rise_coef, fall_coef;
  uint8_t chan;

#if GO32
  speedup_factor = 4.0 * (fps_down_factor + 1);
#else // !GO32
  speedup_factor = 2.5 * 100.0 / sdl_frame_rate;
#endif // !GO32
  speedown_factor = speedup_factor;
  rise_coef = decay_bar_rise * speedup_factor;
  fall_coef = decay_bar_fall * speedown_factor;

  chan = chan_pos - 1;
  for (uint8_t trk = 0; trk < MAX_TRACKS; trk++)
  {
    bool perc = percussion_mode && (chan >= 16) && (chan <= 19);

    // level 1
    decay_bar[chan].lvl1 +=    decay_bar[chan].dir1
                            * (decay_bar[chan].dir1 == 1 ? rise_coef
                                                         : fall_coef);

    if (decay_bar[chan].lvl1 >= carrier_vol[chan]) decay_bar[chan].dir1 = -1;

    if      (decay_bar[chan].lvl1 <  0) decay_bar[chan].lvl1 = 0;
    else if (decay_bar[chan].lvl1 > 63) decay_bar[chan].lvl1 = 63;

    // level 2
    decay_bar[chan].lvl2 +=    decay_bar[chan].dir2
                            * (decay_bar[chan].dir2 == 1 ? rise_coef
                                                         : fall_coef);

    if (decay_bar[chan].lvl2 >= modulator_vol[chan]) decay_bar[chan].dir2 = -1;

    if      (decay_bar[chan].lvl2 <  0) decay_bar[chan].lvl2 = 0;
    else if (decay_bar[chan].lvl2 > 63) decay_bar[chan].lvl2 = 63;

    // volume
    volum_bar[chan].lvl +=    volum_bar[chan].dir
                           * (volum_bar[chan].dir == 1 ? rise_coef
                                                       : fall_coef);
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

    if      (volum_bar[chan].lvl <  0) volum_bar[chan].lvl = 0;
    else if (volum_bar[chan].lvl > 63) volum_bar[chan].lvl = 63;

    if (perc || (ins_parameter (voice_table[chan], 10) & 1))
      self->levels[trk][0] = decay_bar[chan].lvl2;
    else
      self->levels[trk][0] = 63 - (scale_volume (63 - decay_bar[chan].lvl2,
                                                 63 - decay_bar[chan].lvl1));

    self->levels[trk][1] = !perc ? decay_bar[chan].lvl1 : 0;

    chan++;
  }
}

static void decay_bars_window__draw_decay_bars (
  struct decay_bars_window_t *self)
{
  uint8_t attrs[2]; // output screen attributes

  attrs[0] = analyzer + (analyzer_modulator << 4);
  attrs[1] = analyzer + (analyzer_carrier << 4);

  for (uint8_t trk = 0; trk < MAX_TRACKS; trk++)
  {
    decay_bars_window__draw_bar (self, trk, self->levels[trk][0],
      self->area.top_left.x + 7, 1, 6,
      attrs[0]);

    decay_bars_window__draw_bar (self, trk, self->levels[trk][1],
      self->area.top_left.x + 7, 8, 14,
      attrs[1]);
  }
}
