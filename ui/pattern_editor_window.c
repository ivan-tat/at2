// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*** PATTERN EDITOR window ***/

struct pattern_editor_window_t pattern_editor_window =
{
  .area =
  {
    .top_left = { .x = 2, .y = 8 },
    .bottom_right = { .x = 0, .y = 0 }
  },
  .track_width = PATTERN_EDITOR_WINDOW__TRACK_WIDTH_MAX,
  .vol_bar_width = PATTERN_EDITOR_WINDOW__VOL_BAR_WIDTH_MAX
};

static const String *_perc_str[5] =
{
  (String *)"\x02" "BD",
  (String *)"\x02" "SD",
  (String *)"\x02" "TT",
  (String *)"\x02" "TC",
  (String *)"\x02" "HH"
};

// return string must hold 3+1 bytes (3 printable characters)
static String *pattern_editor_window__no_pan_str (
  __UNUSED struct pattern_editor_window_t *self, String *s)
{
  SetLength (s, 3);
  GetStr (s)[0] = charmap.middle_dot; // '·'
  GetStr (s)[1] = charmap.middle_dot; // '·'
  GetStr (s)[2] = ' ';

  return s;
}

// chan: 0..14
// return string must hold 5+1 bytes (3 printable characters)
// return string format: "~cc~ " or "cc "
static String *pattern_editor_window__op_type_str (
  __UNUSED struct pattern_editor_window_t *self, String *s, uint8_t chan)
{
  bool slide = false;

  if ((chan <= 5) || ((chan >= 9) && (chan <= 14)))
  {
    if (songdata.flag_4op &
          (1 << (chan <= 5 ? chan / 2 : 3 + (chan - 9) / 2)))
    {
      SetLength (s, 5);
      GetStr (s)[0] = '~'; // switch color
      GetStr (s)[1] = charmap.four_op_left_half; // '4('
      GetStr (s)[2] = charmap.op_right_half; // ')P'
      if (!vol4op_lock[chan])
      {
        GetStr (s)[3] = '~'; // switch color
        GetStr (s)[4] = ' ';
      }
      else
      {
        GetStr (s)[3] = '+';
        GetStr (s)[4] = '~'; // switch color
      }
    }
    else slide = true;
  } else slide = true;

  if (slide)
  {
    SetLength (s, 3);

    switch (volslide_type[chan])
    {
      case 0:
        GetStr (s)[0] = ' ';
        GetStr (s)[1] = ' ';
        break;
      case 1:
        GetStr (s)[0] = 'C';
        GetStr (s)[1] = charmap.up_down_arrow; // '↕'
        break;
      case 2:
        GetStr (s)[0] = 'M';
        GetStr (s)[1] = charmap.up_down_arrow; // '↕'
        break;
      case 3:
        GetStr (s)[0] = '&';
        GetStr (s)[1] = charmap.up_down_arrow; // '↕'
        break;
      default:
        GetStr (s)[0] = '?';
        GetStr (s)[1] = '?';
        break;
    }

    GetStr (s)[2] = ' ';
  }

  return s;
}

// chan: 15..19
// return string must hold 5+1 bytes (3 printable characters)
// return string format: "~cc~ "
static String *pattern_editor_window__perc_type_str (
  __UNUSED struct pattern_editor_window_t *self, String *s, uint8_t chan)
{
  String t[1+2+1+1+1]; // 2+1 printable characters

  CopyString (t, (String *)"\x01" "~", 1+2+1+1); // "~"
  AppendString (t, (String *)_perc_str[chan - 15], 1+2+1+1);
  AppendString (t, (String *)"\x02" "~ ", 1+2+1+1); // "~ "
  CopyString (s, t, 1+2+1+1);

  return s;
}

// value: 0..?
static String_t pattern_editor_window__vol_bar_str (
  __UNUSED struct pattern_editor_window_t *self, uint8_t value)
{
  String_t r;

  r.len = 0;

  do
  {
    if (value >= charmap.vol_bar_right_steps)
    {
      r.str[r.len] = charmap.vol_bar_right_full;
      r.len++;
      value -= charmap.vol_bar_right_steps;
    }
    if (value && (value < charmap.vol_bar_right_steps))
    {
      r.str[r.len] = charmap.vol_bar_right_tail[value - 1];
      r.len++;
    }
  }
  while (value >= charmap.vol_bar_right_steps);

  return r;
}

// trk: 0..max (MAX_TRACKS, songdata.nm_tracks) - 1
// draws 10 characters
static void pattern_editor_window__draw_track_header_panning_none (
  struct pattern_editor_window_t *self, uint8_t trk)
{
  String s[3+7+1]; // 3+7 printable characters
  String_t t;

  pattern_editor_window__no_pan_str (self, s);
  t = ExpStrR ((String *)"\x00" "", 7, ' ');
  AppendString (s, (String *)&t, 3+7);

  show_str (self->area.top_left.x + 6 + trk * self->track_width + 4,
            self->area.top_left.y + 1,
            s,
            pattern_bckg + pattern_pan_indic);
}

// trk: 0..max (MAX_TRACKS, songdata.nm_tracks) - 1
// chan: 0..5, 9..14
// draws 10 characters
static void pattern_editor_window__draw_track_header_panning_4op (
  struct pattern_editor_window_t *self, uint8_t trk, uint8_t chan)
{
  String s[3+1+7+1+1]; // 3+7 printable characters
  String_t t;
  String t1[2+1];
  uint8_t bit = chan <= 5 ? chan / 2 : 3 + (chan - 9) / 2;

  pattern_editor_window__no_pan_str (self, s);
  AppendString (s, (String *)"\x01" "~", 3+1+7+1);
  if (songdata.flag_4op & (1 << bit))
  {
    SetLength (t1, 2);
    GetStr (t1)[0] = charmap.four_op_left_half; // '4('
    GetStr (t1)[1] = charmap.op_right_half; // ')P'
  }
  else
    SetLength (t1, 0);
  t = ExpStrR (t1, 7, ' ');
  AppendString (s, (String *)&t, 3+1+7+1);
  AppendString (s, (String *)"\x01" "~", 3+1+7+1);

  show_cstr (self->area.top_left.x + 6 + trk * self->track_width + 4,
             self->area.top_left.y + 1,
             s,
             pattern_bckg + pattern_pan_indic,
             pattern_bckg + pattern_4op_indic);
}

// trk: 0..max (MAX_TRACKS, songdata.nm_tracks) - 1
// chan: 15..19
// draws 10 characters
static void pattern_editor_window__draw_track_header_panning_perc (
  struct pattern_editor_window_t *self, uint8_t trk, uint8_t chan)
{
  String s[3+1+7+1+1]; // 3+7 printable characters
  String_t t;

  pattern_editor_window__no_pan_str (self, s);
  AppendString (s, (String *)"\x01" "~", 3+1+7+1);
  t = ExpStrR ((String *)_perc_str[chan - 15], 7, ' ');
  AppendString (s, (String *)&t, 3+1+7+1);
  AppendString (s, (String *)"\x01" "~", 3+1+7+1);

  show_cstr (self->area.top_left.x + 6 + trk * self->track_width + 4,
             self->area.top_left.y + 1,
             s,
             pattern_bckg + pattern_pan_indic,
             pattern_bckg + pattern_perc_indic);
}

// trk: 0..max (MAX_TRACKS, songdata.nm_tracks) - 1
// chan: 0..19
// draws 10 characters
static void pattern_editor_window__draw_track_header_panning_4op_perc (
  struct pattern_editor_window_t *self, uint8_t trk, uint8_t chan)
{
  if (chan <= 5)
    pattern_editor_window__draw_track_header_panning_4op (self, trk, chan);
  else if (chan <= 8)
    pattern_editor_window__draw_track_header_panning_none (self, trk);
  else if (chan <= 14)
    pattern_editor_window__draw_track_header_panning_4op (self, trk, chan);
  else if (chan <= 19)
  {
    if (percussion_mode)
      pattern_editor_window__draw_track_header_panning_perc (self, trk, chan);
    else
      pattern_editor_window__draw_track_header_panning_none (self, trk);
  } else
    pattern_editor_window__draw_track_header_panning_none (self, trk);
}

// trk: 0..max (MAX_TRACKS, songdata.nm_tracks) - 1
// chan: 0..19
// draws 3 characters
static void pattern_editor_window__draw_track_header_panning_lcr (
  struct pattern_editor_window_t *self, uint8_t trk, uint8_t chan)
{
  String s[3+1];
  uint8_t pan;
  uint8_t attr;

  if (panlock && (play_status == isStopped) && !debugging)
    pan = songdata.lock_flags[chan] & 3;
  else
    pan = panning_table[chan];

  SetLength (s, 3);

  switch (pan)
  {
    case 0:
      GetStr (s)[0] = charmap.pan_left; // '(('
      GetStr (s)[1] = charmap.pan_right; // '))'
      break;
    case 1:
      GetStr (s)[0] = charmap.pan_left; // '(('
      GetStr (s)[1] = ' ';
      break;
    case 2:
      GetStr (s)[0] = ' ';
      GetStr (s)[1] = charmap.pan_right; // '))'
      break;
    default:
      GetStr (s)[0] = '?';
      GetStr (s)[1] = '?';
      break;
  }

  GetStr (s)[2] = ' ';

  if (   (!pan_lock[chan])
      || ((play_status == isStopped) && !debugging))
    attr = pattern_bckg + pattern_pan_indic;
  else
    attr = pattern_bckg + pattern_gpan_indic;

  show_str (self->area.top_left.x + 6 + trk * self->track_width + 4,
            self->area.top_left.y + 1,
            s,
            attr);
}

// trk: 0..max (MAX_TRACKS, songdata.nm_tracks) - 1
// chan: 0..19
// draws 7 characters
static void pattern_editor_window__draw_track_header_type (
  struct pattern_editor_window_t *self, uint8_t trk, uint8_t chan)
{
  String s[5+2+2+1]; // 3+2+2 printable characters
  String_t t;
  uint8_t attr;

  if (percussion_mode && (chan >= 15) && (chan <= 19))
    pattern_editor_window__perc_type_str (self, s, chan);
  else
    pattern_editor_window__op_type_str (self, s, chan);

  if (volume_lock[chan])
    AppendString (s, (String *)"\x02" "V+", 5+2+2);
  if (peak_lock[chan])
    AppendString (s, (String *)"\x02" "P+", 5+2+2);

  if (!((chan >= 15) && (chan <= 19)))
    attr = pattern_bckg + pattern_4op_indic;
  else
    attr = pattern_bckg + pattern_perc_indic;

  t = ExpCStrR (s, 7, ' ');
  show_cstr (self->area.top_left.x + 6 + trk * self->track_width + 4 + 3,
             self->area.top_left.y + 1,
             (String *)&t,
             pattern_bckg + pattern_lock_indic,
             attr);
}

// trk: 0..max (MAX_TRACKS, songdata.nm_tracks) - 1
// chan: 0..19
static void pattern_editor_window__draw_track_header (
  struct pattern_editor_window_t *self, uint8_t trk, uint8_t chan)
{
  if ((!((play_status == isStopped) && (!debugging) && (!panlock)))
      && channel_flag[chan])
  {
    pattern_editor_window__draw_track_header_panning_lcr (self, trk, chan);
    pattern_editor_window__draw_track_header_type (self, trk, chan);
  }
  else
    pattern_editor_window__draw_track_header_panning_4op_perc (self, trk, chan);
}

// trk: 0..max (MAX_TRACKS, songdata.nm_tracks) - 1
static void pattern_editor_window__clear_track_footer (
  struct pattern_editor_window_t *self, uint8_t trk)
{
  String_t t;

/*
  t = ExpStrR ((String *)"\x00" "",
               self->track_width - 1,
               charmap.vol_bar_right_back); // "━"..."━"
*/
  t = Copy ((String *)(&patt_win[4]),
            1 + 6 + trk * self->track_width,
            self->track_width);

  show_str (self->area.top_left.x + 6 + trk * self->track_width,
            self->area.bottom_right.y,
            (String *)&t,
            pattern_bckg + pattern_border);
}

// trk: 0..max (MAX_TRACKS, songdata.nm_tracks) - 1
static void pattern_editor_window__draw_track_footer_off_flag (
  struct pattern_editor_window_t *self, uint8_t trk)
{
  String s[3+1+8+1+4+1]; // 3+8+4 printable characters
  String t[4+1];

  SetLength (s, 3);
  GetStr (s)[0] = charmap.vol_bar_right_back;
  GetStr (s)[1] = charmap.vol_bar_right_back;
  GetStr (s)[2] = charmap.vol_bar_right_back;
  AppendString (s, (String *)"\x0A" "~ () OFF ~", 3+1+8+1+4);
  SetLength (t, 4);
  GetStr (t)[0] = charmap.vol_bar_right_back;
  GetStr (t)[1] = charmap.vol_bar_right_back;
  GetStr (t)[2] = charmap.vol_bar_right_back;
  GetStr (t)[3] = trk < max (MAX_TRACKS, songdata.nm_tracks) - 1 ? charmap.vol_bar_right_back2
                                                                 : charmap.bd_heavy_up_and_horiz;
  AppendString (s, t, 3+1+8+1+4);

  show_cstr (self->area.top_left.x + 6 + trk * self->track_width,
             self->area.bottom_right.y,
             s,
             pattern_bckg + pattern_border,
             pattern_bckg + pattern_chan_indic);
}

// trk: 0..max (MAX_TRACKS, songdata.nm_tracks) - 1
// chan: 0..19
static void pattern_editor_window__draw_track_footer_volume_bar (
  struct pattern_editor_window_t *self, uint8_t trk, uint8_t chan)
{
  uint8_t last_trk = max (MAX_TRACKS, songdata.nm_tracks) - 1;
  String_t s;
  String t[1+1];

  if (  (!is_4op_chan (chan + 1))
      || (is_4op_chan (chan + 1) && bit_test (_4op_tracks_lo, chan + 1) && (trk == 0))
      ||                           (bit_test (_4op_tracks_hi, chan + 1) && (trk == last_trk)))
  {
    // scale `level' to fit in `self->vol_bar_width' characters
    // (`charmap.vol_bar_right_steps' lines each, `+ 31' for rounding)
    s = pattern_editor_window__vol_bar_str (self,
          (uint8_t)(((double)volum_bar[chan].lvl
          * self->vol_bar_width
          * charmap.vol_bar_right_steps + 31) / 63));

    s = ExpStrR ((String *)&s, self->vol_bar_width,
                 charmap.vol_bar_right_back);

    SetLength (t, 1);
    GetStr (t)[0] = trk < last_trk ? charmap.vol_bar_right_back2
                                   : charmap.bd_heavy_up_and_horiz;
    AppendString ((String *)&s, t, 255);

    show_str (self->area.top_left.x + 6 + trk * self->track_width,
              self->area.bottom_right.y,
              (String *)&s,
              pattern_bckg + pattern_border);
  }
  else if (bit_test (_4op_tracks_hi, chan + 1))
  {
    bool large;

    // scale `level' to fit in `self->vol_bar_width * 2' characters
    // (`charmap.vol_bar_right_steps' lines each, `+ 31' for rounding)
    s = pattern_editor_window__vol_bar_str (self,
          (uint8_t)(((double)volum_bar[chan].lvl
          * self->vol_bar_width * 2
          * charmap.vol_bar_right_steps + 31) / 63));

    large = s.len > self->vol_bar_width;
    if (large)
      s.str[self->vol_bar_width] = charmap.vol_bar_right_full2;

    s = ExpStrR ((String *)&s, self->vol_bar_width * 2 + 1,
                 charmap.vol_bar_right_back);

    if (!large)
      s.str[self->vol_bar_width] = charmap.vol_bar_right_back2;

    SetLength (t, 1);
    GetStr (t)[0] = trk + 1 < last_trk ? charmap.vol_bar_right_back2
                                       : charmap.bd_heavy_up_and_horiz;
    AppendString ((String *)&s, t, 255);

    show_str (self->area.top_left.x + 6 + trk * self->track_width,
              self->area.bottom_right.y,
              (String *)&s,
              pattern_bckg + pattern_border);
  }
}

static void pattern_editor_window__clear_all_tracks_footer_volume_bars (
  struct pattern_editor_window_t *self)
{
  uint8_t chan = chan_pos - 1;

  for (uint8_t trk = 0; trk < MAX_TRACKS; trk++)
  {
    if (channel_flag[chan])
      pattern_editor_window__clear_track_footer (self, trk);

    chan++;
  }
}

// trk: 0..max (MAX_TRACKS, songdata.nm_tracks) - 1
// chan: 0..19
static void pattern_editor_window__draw_track_footer (
  struct pattern_editor_window_t *self, uint8_t trk, uint8_t chan)
{
  if (channel_flag[chan])
  {
#if GO32
    if (is_scrollable_screen_mode () && (scr_scroll_y <= font_height * 1))
#else // !GO32
    if (is_default_screen_mode () && (screen_scroll_offset <= font_height * 1))
#endif // !GO32
      pattern_editor_window__draw_track_footer_volume_bar (self, trk, chan);
    else
      pattern_editor_window__clear_track_footer (self, trk);
  }
  else
    pattern_editor_window__draw_track_footer_off_flag (self, trk);
}

static void pattern_editor_window__status_refresh (
  struct pattern_editor_window_t *self)
{
  if (no_status_refresh)
  {
    pattern_editor_window__clear_all_tracks_footer_volume_bars (self);
  }
  else
  {
    if (single_play) return;
  }
}

static void pattern_editor_window__status_refresh2 (
  struct pattern_editor_window_t *self)
{
  uint8_t last_trk = max (MAX_TRACKS, songdata.nm_tracks) - 1;
  uint8_t chan = chan_pos - 1;

  for (uint8_t trk = 0; trk <= last_trk; trk++)
  {
    pattern_editor_window__draw_track_header (self, trk, chan);
    pattern_editor_window__draw_track_footer (self, trk, chan);
    chan++;
  }
}
