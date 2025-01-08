// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*** MAIN window ***/

struct main_window_t main_window =
{
  .area =
  {
    .top_left = { .x = 1, .y = 1 },
    .bottom_right = { .x = 0, .y = 0 }
  }
};

// Play status
static void main_window__draw_play_status (struct main_window_t *self,
  bool show_optional)
{
  String_t s; // 12+1 bytes

  if (show_optional)
  {
    String_t t; // 10+1 bytes

    s.len = 1;
    s.str[0] = ' ';
    CopyString ((String *)&t, status_layout[play_status], 12);
    t = ExpStrR ((String *)&t, 10, ' ');
    AppendString ((String *)&s, (String *)&t, 12);
  }
  else
    s = ExpStrR ((String *)"\x00" "", 12, charmap.bd_light_horiz); // "─".."─"

  show_str (self->area.top_left.x + 1,
            self->area.top_left.y,
            (String *)&s,
            main_background + main_border);
}

#if !GO32

// WAV indicator
static void main_window__draw_wav_indicator (struct main_window_t *self,
  bool show_optional)
{
  if (show_optional)
  {
    String s[3+1];
    uint8_t attr;

    if (sdl_opl3_emulator)
    {
      CopyString (s, (String *)"\x03" "WAV", 3);
      if ((play_status == isPlaying) && !opl3_channel_recording_mode)
        attr = (main_behavior << 4) & 0xF0;
      else
        attr = main_background + main_behavior;
    }
    else
    {
      SetLength (s, 3);
      GetStr (s)[0] = charmap.bd_light_horiz; // '─'
      GetStr (s)[1] = charmap.bd_light_horiz; // '─'
      GetStr (s)[2] = charmap.bd_light_horiz; // '─'
      attr = main_background + main_border;
    }

    show_str (self->area.top_left.x + 12,
              self->area.top_left.y,
              s,
              attr);
  }
}

#endif // !GO32

// Playing time
static void main_window__draw_playing_time (struct main_window_t *self,
  bool show_optional)
{
  uint8_t attrs[2];

  attrs[0] = main_background + main_border;
  attrs[1] = (main_border << 4) & 0xF0;

  if (show_optional)
  {
    String s[13+1];

    _time_playing_str (s); // 13 characters

    show_cstr (self->area.top_left.x + 15,
               self->area.top_left.y,
               s,
               attrs[_IRQFREQ_blink_flag && _IRQFREQ_update_event ? 1 : 0],
               attrs[0]);
  }
  else
  {
    String_t s; // 11+1 bytes

    s = ExpStrR ((String *)"\x00" "", 11, charmap.bd_light_horiz); // "─"..."─"

    show_str (self->area.top_left.x + 15,
              self->area.top_left.y,
              (String *)&s,
              attrs[0]);
  }
}

// Current octave
static void main_window__draw_current_octave (struct main_window_t *self)
{
  String s[8+2+1]; // 8 printable characters
  int i = 0;

  SetLength (s, 8+2);

  for (uint8_t octave = 1; octave <= 8; octave++)
  {
    char c = '0' + octave;

    if (octave != current_octave)
    {
      GetStr (s)[i] = c;
      i++;
    }
    else
    {
      GetStr (s)[i + 0] = '~'; // switch color
      GetStr (s)[i + 1] = c;
      GetStr (s)[i + 2] = '~'; // switch color
      i += 1 + 2;
    }
  }

  show_cstr (self->area.top_left.x + 30,
             self->area.bottom_right.y,
             s,
             main_background + main_stat_line,
             main_background + main_hi_stat_line);
}

// 2OP/4OP flag
static void main_window__draw_4op_flag (struct main_window_t *self)
{
  String s[8+1]; // 4 printable characters

  SetLength (s, 8);
  GetStr (s)[0] = '~'; // switch color
  GetStr (s)[1] = '[';
  GetStr (s)[2] = '~'; // switch color
  GetStr (s)[3] = songdata.flag_4op ? charmap.four_op_left_half
                                    : charmap.two_op_left_half;
  GetStr (s)[4] = charmap.op_right_half; // ')P'
  GetStr (s)[5] = '~'; // switch color
  GetStr (s)[6] = ']';
  GetStr (s)[7] = '~'; // switch color

  show_cstr (self->area.bottom_right.x - 43,
             self->area.bottom_right.y,
             s,
             main_background + main_hi_stat_line,
             main_background + main_stat_line);
}

// "Cycle pattern" flag
static void main_window__draw_cycle_pattern_flag (struct main_window_t *self)
{
  String s[1+1];
  uint8_t attr;

  SetLength (s, 1);

  if (command_typing == 1)
  {
    GetStr (s)[0] = !cycle_pattern ? 'A' : 'F';
    attr = main_background + main_behavior;
  }
  else if ((command_typing == 2) && !cycle_pattern)
  {
    GetStr (s)[0] = 'S';
    attr = main_background + main_behavior;
  }
  else
  {
    GetStr (s)[0] = charmap.bd_light_horiz; // "─"
    attr = main_background + main_border;
  }

  show_str (self->area.bottom_right.x - 17,
            self->area.bottom_right.y,
            s,
            attr);
}

static void main_window__status_refresh (struct main_window_t *self,
  bool show_optional)
{
  if (no_status_refresh)
  {
    // nothing
  }
  else
  {
    main_window__draw_play_status (self, show_optional);

    if (single_play) return;

#if !GO32
    main_window__draw_wav_indicator (self, show_optional);
#endif // !GO32
    main_window__draw_playing_time (self, show_optional);

    if (no_status_refresh) return;
  }
}

static void main_window__status_refresh2 (struct main_window_t *self)
{
  main_window__draw_cycle_pattern_flag (self);
  main_window__draw_4op_flag (self);
  main_window__draw_current_octave (self);
}
