// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void TxtScrIO_Init (void) {
  uint8_t temp;
  int i;

#if GO32
  CopyString (_last_debug_str_, _debug_str_, 255);
  StrToString (_debug_str_, __FILE__ ":" "TxtScrIO_Init", 255);
  program_screen_mode = screen_mode;
#endif // GO32

  mn_environment.v_dest = screen_ptr;
  centered_frame_vdest = screen_ptr;

#if GO32
  if (!is_VESA_emulated_mode ()) {
#endif // GO32
    switch (program_screen_mode) {
    default:
      break;
    case 0:
      SCREEN_RES_X = 720;
      SCREEN_RES_Y = 480;
      MAX_COLUMNS = 90;
      MAX_ROWS = 40;
      MAX_ORDER_COLS = 9;
      MAX_TRACKS = 5;
      MAX_PATTERN_ROWS = 18;
      INSCTRL_xshift = 0;
      INSCTRL_yshift = 0;
      INSEDIT_yshift = 0;
      PATTORD_xshift = 0;
      MaxCol = MAX_COLUMNS;
      MaxLn = MAX_ROWS;
      hard_maxcol = MAX_COLUMNS;
      hard_maxln = 30;
      work_MaxCol = MAX_COLUMNS;
      work_MaxLn = 30;
      scr_font_width = 8;
      scr_font_height = 16;
      break;
    // full-screen view
    case 1:
      SCREEN_RES_X = 960;
      SCREEN_RES_Y = 800;
      MAX_COLUMNS = 120;
      MAX_ROWS = 50;
      MAX_ORDER_COLS = 13;
      MAX_TRACKS = 7;
      MAX_PATTERN_ROWS = 28;
      INSCTRL_xshift = 15;
      INSCTRL_yshift = 6;
      INSEDIT_yshift = 12;
      PATTORD_xshift = 1;
      MaxCol = MAX_COLUMNS;
      MaxLn = MAX_ROWS;
      hard_maxcol = MAX_COLUMNS;
      hard_maxln = 50;
      work_MaxCol = MAX_COLUMNS;
      work_MaxLn = 40;
      scr_font_width = 8;
      scr_font_height = 16;
      break;
    // wide full-screen view
    case 2:
      SCREEN_RES_X = 1440;
      SCREEN_RES_Y = 960;
      MAX_COLUMNS = 180;
      MAX_ROWS = 60;
      MAX_ORDER_COLS = 22;
      MAX_TRACKS = 11;
      MAX_PATTERN_ROWS = 38;
      INSCTRL_xshift = 45;
      INSCTRL_yshift = 12;
      INSEDIT_yshift = 12;
      PATTORD_xshift = 0;
      MaxCol = MAX_COLUMNS;
      MaxLn = MAX_ROWS;
      hard_maxcol = MAX_COLUMNS;
      hard_maxln = 60;
      work_MaxCol = MAX_COLUMNS;
      work_MaxLn = 50;
      scr_font_width = 8;
      scr_font_height = 16;
      break;
    // 90x47
    case 4:
      SCREEN_RES_X = 800;
      SCREEN_RES_Y = 600;
      MAX_COLUMNS = 90;
      MAX_ROWS = 38;
      MAX_ORDER_COLS = 9;
      MAX_TRACKS = 5;
      MAX_PATTERN_ROWS = 16;
      INSCTRL_xshift = 0;
      INSCTRL_yshift = 4;
      INSEDIT_yshift = 12;
      PATTORD_xshift = 0;
      MaxCol = MAX_COLUMNS;
      MaxLn = MAX_ROWS;
      hard_maxcol = MAX_COLUMNS;
      hard_maxln = 38;
      work_MaxCol = MAX_COLUMNS;
      work_MaxLn = 38;
      scr_font_width = 8;
      scr_font_height = 16;
      GOTOXY_xshift = ((SCREEN_RES_X / scr_font_width) - MAX_COLUMNS) / 2;
      break;
    // 120x47
    case 5:
      SCREEN_RES_X = 1024;
      SCREEN_RES_Y = 768;
      MAX_COLUMNS = 120;
      MAX_ROWS = 48;
      MAX_ORDER_COLS = 13;
      MAX_TRACKS = 7;
      MAX_PATTERN_ROWS = 26;
      INSCTRL_xshift = 15;
      INSCTRL_yshift = 7;
      INSEDIT_yshift = 12;
      PATTORD_xshift = 1;
      MaxCol = MAX_COLUMNS;
      MaxLn = MAX_ROWS;
      hard_maxcol = MAX_COLUMNS;
      hard_maxln = 48;
      work_MaxCol = MAX_COLUMNS;
      work_MaxLn = 48;
      scr_font_width = 8;
      scr_font_height = 16;
      GOTOXY_xshift = ((SCREEN_RES_X / scr_font_width) - MAX_COLUMNS) / 2;
      break;
#if GO32
    // compatibility text-mode
    case 3:
      switch (comp_text_mode) {
      case 0:
      case 1:
        SCREEN_RES_X = 720;
        SCREEN_RES_Y = 480;
        MAX_COLUMNS = 90;
        MAX_ROWS = 40;
        MAX_ORDER_COLS = 9;
        MAX_TRACKS = 5;
        MAX_PATTERN_ROWS = 18;
        INSCTRL_xshift = 0;
        INSCTRL_yshift = 0;
        PATTORD_xshift = 0;
        INSEDIT_yshift = 0;
        MaxCol = MAX_COLUMNS;
        MaxLn = MAX_ROWS;
        hard_maxcol = MAX_COLUMNS;
        hard_maxln = 30;
        work_MaxCol = MAX_COLUMNS;
        work_MaxLn = 30;
        scr_font_width = 9;
        scr_font_height = 16;
        break;
      default:
        break;
      }
      break;
    }
  } else
    // VESA-emulated text-mode
    switch (get_VESA_emulated_mode_idx ()) {
    default:
      break;
    // 90x30 (default mode)
    case 0:
      SCREEN_RES_X = 800;
      SCREEN_RES_Y = 600;
      MAX_COLUMNS = 90;
      MAX_ROWS = 40;
      MAX_ORDER_COLS = 9;
      MAX_TRACKS = 5;
      MAX_PATTERN_ROWS = 18;
      INSCTRL_xshift = 0;
      INSCTRL_yshift = 0;
      INSEDIT_yshift = 0;
      PATTORD_xshift = 0;
      MaxCol = MAX_COLUMNS;
      MaxLn = MAX_ROWS;
      hard_maxcol = MAX_COLUMNS;
      hard_maxln = 30;
      work_MaxCol = MAX_COLUMNS;
      work_MaxLn = 30;
      scr_font_width = 8;
      scr_font_height = 16;
      break;
    // 90x47
    case 1:
      SCREEN_RES_X = 800;
      SCREEN_RES_Y = 600;
      MAX_COLUMNS = 90;
      MAX_ROWS = 46;
      MAX_ORDER_COLS = 9;
      MAX_TRACKS = 5;
      MAX_PATTERN_ROWS = 24;
      INSCTRL_xshift = 0;
      INSCTRL_yshift = 4;
      INSEDIT_yshift = 0;
      PATTORD_xshift = 0;
      MaxCol = MAX_COLUMNS;
      MaxLn = MAX_ROWS;
      hard_maxcol = MAX_COLUMNS;
      hard_maxln = 36;
      work_MaxCol = MAX_COLUMNS;
      work_MaxLn = 36;
      scr_font_width = 8;
      scr_font_height = 16;
      break;
    // 120x47
    case 2:
      SCREEN_RES_X = 1024;
      SCREEN_RES_Y = 768;
      MAX_COLUMNS = 120;
      MAX_ROWS = 46;
      MAX_ORDER_COLS = 13;
      MAX_TRACKS = 7;
      MAX_PATTERN_ROWS = 24;
      INSCTRL_xshift = 15;
      INSCTRL_yshift = 7;
      INSEDIT_yshift = 12;
      PATTORD_xshift = 1;
      MaxCol = MAX_COLUMNS;
      MaxLn = MAX_ROWS;
      hard_maxcol = MAX_COLUMNS;
      hard_maxln = 47;
      work_MaxCol = MAX_COLUMNS;
      work_MaxLn = 36;
      scr_font_width = 8;
      scr_font_height = 16;
      break;
    }
#else // !GO32
  }
#endif // !GO32

  memsetw (screen_ptr, 0x0700, MAX_SCREEN_MEM_SIZE / 2);
  SCREEN_MEM_SIZE = (SCREEN_RES_X / scr_font_width) * MAX_ROWS * 2;
  move_to_screen_routine = move2screen;

  if (!command_typing)
    _pattedit_lastpos = 4 * MAX_TRACKS;
  else
    _pattedit_lastpos = 10 * MAX_TRACKS;

  switch (MAX_COLUMNS) {
  case 120:
    temp = 1;
    break;
  case 180:
    temp = 2;
    break;
  default:
    temp = 0;
    break;
  }

  for (i = 0; i < 5; i++) {
    char s[176+1];

    UTF8nstr_to_AT2 (s, 176, patt_win_tracks[temp][i]);
    StrToString (patt_win[i], s, 176);
  }
}
