// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*** STATUS window ***/

struct status_window_t status_window =
{
  .area =
  {
    .top_left = { .x = 2, .y = 2 },
    .bottom_right = { .x = 24, .y = 7 }
  }
};

// Play status
static void status_window__draw_play_status (struct status_window_t *self)
{
  String_t s; // 10+1 bytes

  CopyString ((String *)&s, status_layout[play_status], 10);
  s = ExpStrR ((String *)&s, 10, ' ');

  show_str (self->area.top_left.x + 12,
            self->area.top_left.y + 4,
            (String *)&s,
            status_background + status_play_state);
}

// Play status: blink " >" in "Playing >"
static void status_window__blink_play_status (struct status_window_t *self)
{
  String s[2+1];

  SetLength (s, 2);
  GetStr (s)[0] = status_layout[play_status][8];
  GetStr (s)[1] = status_layout[play_status][9];

  show_str (self->area.top_left.x + 12 + 7,
            self->area.top_left.y + 4,
            s,
            status_background + status_play_state);
}

// Current pattern number and loop flag
static void status_window__draw_current_pattern (struct status_window_t *self)
{
  String_t s; // 4+1 bytes
  uint8_t attr;

  if (play_single_patt)
  {
    s = byte2hex (current_pattern);
    AppendString ((String *)&s, (String *)"\x02" "  ", 4);
    attr = pattern_bckg + status_dynamic_txt;
  }
  else
  {
    uint8_t n = songdata.pattern_order[current_order];

    if (n >= 0x80) n = 0;
    s = byte2hex (n);

    if (!calibrating)
    {
      String t[2+1];

      SetLength (t, 2);
      GetStr (t)[0] = ' ';

      if ((!replay_forbidden) && repeat_pattern)
      {
        GetStr (t)[1] = charmap.loop_flag;
        attr = pattern_bckg + status_dynamic_txt;
      }
      else
      {
        GetStr (t)[1] = ' ';
        attr = status_background + status_dynamic_txt;
      }

      AppendString ((String *)&s, (String *)&t, 4);
    }
    else
      attr = status_background + status_dynamic_txt;
  }

  show_str (self->area.top_left.x + 18,
            self->area.top_left.y + 1,
            (String *)&s,
            attr);
}

// Current order number
static void status_window__draw_current_order (struct status_window_t *self)
{
  String_t s; // 2+1 bytes
  const String *ps;

  if (!play_single_patt)
  {
    s = byte2hex (current_order);
    ps = (String *)&s;
  }
  else
    ps = (String *)"\x02" "--";

  show_str (self->area.top_left.x + 15,
            self->area.top_left.y + 1,
            (String *)ps,
            status_background + status_dynamic_txt);
}

// Current line number
static void status_window__draw_current_line (struct status_window_t *self)
{
  String_t t; // 2+1 bytes

  t = byte2hex (current_line);

  show_str (self->area.top_left.x + 15,
            self->area.top_left.y + 2,
            (String *)&t,
            status_background + status_dynamic_txt);
}

// BPM or speed with tempo
static void status_window__draw_current_speed (struct status_window_t *self)
{
  if (_IRQFREQ_update_event && !_show_bpm_realtime_proc)
  {
    double bpm = calc_realtime_bpm_speed (tempo, speed, mark_line);
    String_t s, t; // 7+1 bytes

    t = Bpm2str (bpm);
    s = ExpStrR ((String *)&t, 7, ' ');

    show_str (self->area.top_left.x + 1,
              self->area.top_left.y + 3,
              (String *)"\x0C" "SPEED iN BPM",
              status_background + status_static_txt);
    show_str (self->area.top_left.x + 15,
              self->area.top_left.y + 3,
              (String *)&s,
              status_background + status_dynamic_txt);
  }
  else if (!_IRQFREQ_update_event)
  {
    String_t s; // 4+1 bytes
    String t[2+1];

    s = byte2hex (speed);

    show_str (self->area.top_left.x + 1,
              self->area.top_left.y + 3,
              (String *)"\x11" "SPEED/TEMPO     /",
              status_background + status_static_txt);
    show_str (self->area.top_left.x + 15,
              self->area.top_left.y + 3,
              (String *)&s,
              status_background + status_dynamic_txt);

    s = byte2dec (tempo);
    if (tempo < 100)
    {
      SetLength (t, 2);

      if ((tempo == PIT_FREQ_FLOOR) && timer_fix)
      {
        GetStr (t)[0] = charmap.point_two; // '.2'
        GetStr (t)[1] = charmap.hertz; // 'Hz'
      }
      else
      {
        GetStr (t)[0] = charmap.hertz; // 'Hz'
        GetStr (t)[1] = ' ';
      }
    }
    else
    {
      SetLength (t, 1);
      GetStr (t)[0] = charmap.hertz; // 'Hz'
    }
    AppendString ((String *)&s, (String *)&t, 4); // "Hz"

    show_str (self->area.top_left.x + 18,
              self->area.top_left.y + 3,
              (String *)&s,
              status_background + status_dynamic_txt);
  }
}

// Playing time or IRQ frequency update event
static void status_window__draw_playing_time (struct status_window_t *self)
{
  String_t s; // 19+1 bytes
  uint8_t attrs1[2], attrs2[2];

  attrs1[0] = status_background + status_text_dis;
  attrs1[1] = (status_border << 4) & 0xF0;
  attrs2[0] = status_background + status_border;
  attrs2[1] = status_background + status_border;

  if (_IRQFREQ_update_event)
  {
    String_t t; // 10+1 bytes
    String t1[1+1];

    CopyString ((String *)&s, (String *)"\x04" "~ ~ ", 255);

    if ((IRQ_freq_shift + playback_speed_shift) > 0)
    {
      String_t t1; // 11+1 bytes

      CopyString ((String *)&t, (String *)"\x01" "+", 255);
      t1 = Num2str (IRQ_freq_shift + playback_speed_shift, 10);
      AppendString ((String *)&t, (String *)&t1, 255);
    }
    else if ((IRQ_freq_shift + playback_speed_shift) < 0)
    {
      String_t t1; // 11+1 bytes

      CopyString ((String *)&t, (String *)"\x01" "-", 255);
      t1 = Num2str (abs (IRQ_freq_shift + playback_speed_shift), 10);
      AppendString ((String *)&t, (String *)&t1, 255);
    }
    else
      t = Num2str (IRQ_freq + IRQ_freq_shift + playback_speed_shift, 10);

    SetLength (t1, 1);
    GetStr (t1)[0] = charmap.hertz; // 'Hz'
    AppendString ((String *)&t, (String *)&t1, 255);
    t = CenterStr ((String *)&t, 5);
    AppendString ((String *)&s, (String *)&t, 255);
    AppendString ((String *)&s, (String *)"\x04" " ~ ~", 255);
  }
  else
    _time_playing_str ((String *)&s); // 13 characters

  show_cstr (self->area.top_left.x + 11,
             self->area.bottom_right.y,
             (String *)&s,
             attrs1[_IRQFREQ_blink_flag && _IRQFREQ_update_event ? 1 : 0],
             attrs2[_IRQFREQ_blink_flag && _IRQFREQ_update_event ? 1 : 0]);
}

// File name w/o extension
static void status_window__draw_base_name (struct status_window_t *self)
{
  String s[10+1]; // 10+1 bytes
  String_t t; // 8+1 bytes
#if !GO32
  String_t _name; // MAX_NAME_LEN+1 bytes
  ssize_t _name_len;

  _name = FilterStr2 ((String *)&songdata_title, _valid_characters_fname, '_');
  _name = BaseNameOnly ((String *)&_name);
  _name_len = _name.len;

  while (((8 + _name_scrl_shift) > _name_len) && (_name_scrl_shift > 0))
    _name_scrl_shift--;
#endif // !GO32

  CopyString (s, (String *)"\x01" "[", 10);
#if GO32
  t = Copy ((String *)&songdata_title, 1,
            PosChr ('.', (String *)&songdata_title) - 1);
#else // !GO32
  t = Copy ((String *)&_name, 1 + _name_scrl_shift, 8);
#endif // !GO32
  t = iCASE ((String *)&t);
  t = ExpStrR ((String *)&t, 8, ' ');
  AppendString (s, (String *)&t, 10);
  AppendString (s, (String *)"\x01" "]", 10);

  show_str (self->area.top_left.x + 1,
            self->area.top_left.y + 4,
            s,
            status_background + status_dynamic_txt);

#if !GO32
  // scroll name

  if (_name_scrl_pending_frames)
    _name_scrl_pending_frames--;
  else
  {
    _name_scrl_pending_frames = 20;

    if (_name_scrl_shift_ctr > 0)
    {
      if ((8 + _name_scrl_shift) < _name_len)
        _name_scrl_shift += _name_scrl_shift_ctr;
      else
      {
        _name_scrl_shift_ctr = -1;
        _name_scrl_pending_frames *= 10;
      }
    }
    else if (_name_scrl_shift > 0)
      _name_scrl_shift += _name_scrl_shift_ctr;
    else
    {
      _name_scrl_shift_ctr = 1;
      _name_scrl_pending_frames *= 50;
    }
  }
#endif // !GO32
}

// File name extension
static void status_window__draw_extension (struct status_window_t *self)
{
  String s[5+1];
  String_t t; // 3 bytes

  CopyString (s, (String *)"\x01" "[", 5);
#if GO32
  t = Copy ((String *)&songdata_title,
            PosChr ('.', (String *)&songdata_title) + 1, 3);
#else // GO32
  t = ExtOnly ((String *)&songdata_title);
  t = Copy ((String *)&t, 1, 3);
#endif // !GO32
  t = iCASE ((String *)&t);
  t = ExpStrR ((String *)&t, 3, ' ');
  AppendString (s, (String *)&t, 5);
  AppendString (s, (String *)"\x01" "]", 5);

  show_str (self->area.top_left.x + 1,
            self->area.bottom_right.y,
            s,
            status_background + status_border);
}

#if !GO32

// WAV indicator
static void status_window__draw_wav_indicator (struct status_window_t *self)
{
  uint8_t attrs[2];

  attrs[0] = main_background + main_behavior;
  if ((play_status == isPlaying) && !opl3_channel_recording_mode)
    attrs[1] = (main_behavior << 4) & 0xF0;
  else
    attrs[1] = main_background + main_behavior;

  show_str (self->area.top_left.x + 6,
            self->area.bottom_right.y,
            (String *)"\x05" "[WAV]",
            attrs[_WAVREC_blink_flag ? 1 : 0]);
}

#endif // !GO32

// "Module archived" flag
static void status_window__draw_module_archived_flag (
  struct status_window_t *self)
{
  if (!module_archived)
  {
#if GO32
    String s[3+1];

    SetLength (s, 3);
    GetStr (s)[0] = '[';
    GetStr (s)[1] = charmap.floppy_disk;
    GetStr (s)[2] = ']';
    show_str (self->area.top_left.x + 6,
              self->area.bottom_right.y,
              s,
              main_background + main_behavior);
#else // !GO32
    String s[7+1];

    SetLength (s, 7);
    GetStr (s)[0] = '~'; // switch color
    GetStr (s)[1] = '[';
    GetStr (s)[2] = charmap.floppy_disk; // '🖫'
    GetStr (s)[3] = ']';
    GetStr (s)[4] = '~'; // switch color
    GetStr (s)[5] = charmap.bd_heavy_horiz; // '━'
    GetStr (s)[6] = charmap.bd_heavy_horiz; // '━'
    show_cstr (self->area.top_left.x + 6,
               self->area.bottom_right.y,
               s,
               status_background + status_border,
               main_background + main_behavior);
#endif // !GO32
  }
  else
  {
#if GO32
    String s[3+1];

    SetLength (s, 3);
#else // !GO32
    String s[5+1];

    SetLength (s, 5);
#endif // !GO32

    GetStr (s)[0] = charmap.bd_heavy_horiz; // '━'
    GetStr (s)[1] = charmap.bd_heavy_horiz; // '━'
    GetStr (s)[2] = charmap.bd_heavy_horiz; // '━'
#if !GO32
    GetStr (s)[3] = charmap.bd_heavy_horiz; // '━'
    GetStr (s)[4] = charmap.bd_heavy_horiz; // '━'
#endif // !GO32

    show_str (self->area.top_left.x + 6,
              self->area.bottom_right.y,
              s,
              status_background + status_border);
  }
}

static void status_window__status_refresh (struct status_window_t *self,
  __UNUSED bool show_optional)
{
  if (no_status_refresh)
  {
    status_window__blink_play_status (self);
  }
  else
  {
    status_window__draw_play_status (self);

    if (single_play) return;

    status_window__draw_current_pattern (self);
    status_window__draw_current_order (self);
    status_window__draw_current_line (self);
    status_window__draw_current_speed (self);
    status_window__draw_playing_time (self);
    status_window__draw_base_name (self);
    status_window__draw_extension (self);
#if GO32
    status_window__draw_module_archived_flag (self);
#else // !GO32
    if (show_optional)
    {
      if (sdl_opl3_emulator)
        status_window__draw_wav_indicator (self);
      else
        status_window__draw_module_archived_flag (self);
    }
#endif // !GO32
  }
}

static void status_window__status_refresh2 (
  __UNUSED struct status_window_t *self)
{
  // nothing
}
