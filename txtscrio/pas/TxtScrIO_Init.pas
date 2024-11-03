// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure TxtScrIO_Init;

var
  temp: Byte;

begin
  _dbg_enter ({$I %FILE%}, 'TxtScrIO_Init');

{$IFDEF GO32V2}
  program_screen_mode := screen_mode;
{$ENDIF}

  mn_environment.v_dest := screen_ptr;
  centered_frame_vdest := screen_ptr;

{$IFDEF GO32V2}
  If NOT is_VESA_emulated_mode then
{$ENDIF}
    Case program_screen_mode of
      0: begin
           SCREEN_RES_X := 720;
           SCREEN_RES_Y := 480;
           MAX_COLUMNS := 90;
           MAX_ROWS := 40;
           MAX_ORDER_COLS := 9;
           MAX_TRACKS := 5;
           MAX_PATTERN_ROWS := 18;
           INSCTRL_xshift := 0;
           INSCTRL_yshift := 0;
           INSEDIT_yshift := 0;
           PATTORD_xshift := 0;
           MaxCol := MAX_COLUMNS;
           MaxLn := MAX_ROWS;
           hard_maxcol := MAX_COLUMNS;
           hard_maxln := 30;
           work_MaxCol := MAX_COLUMNS;
           work_MaxLn := 30;
           scr_font_width := 8;
           scr_font_height := 16;
         end;
      // full-screen view
      1: begin
           SCREEN_RES_X := 960;
           SCREEN_RES_Y := 800;
           MAX_COLUMNS := 120;
           MAX_ROWS := 50;
           MAX_ORDER_COLS := 13;
           MAX_TRACKS := 7;
           MAX_PATTERN_ROWS := 28;
           INSCTRL_xshift := 15;
           INSCTRL_yshift := 6;
           INSEDIT_yshift := 12;
           PATTORD_xshift := 1;
           MaxCol := MAX_COLUMNS;
           MaxLn := MAX_ROWS;
           hard_maxcol := MAX_COLUMNS;
           hard_maxln := 50;
           work_MaxCol := MAX_COLUMNS;
           work_MaxLn := 40;
           scr_font_width := 8;
           scr_font_height := 16;
         end;
      // wide full-screen view
      2: begin
           SCREEN_RES_X := 1440;
           SCREEN_RES_Y := 960;
           MAX_COLUMNS := 180;
           MAX_ROWS := 60;
           MAX_ORDER_COLS := 22;
           MAX_TRACKS := 11;
           MAX_PATTERN_ROWS := 38;
           INSCTRL_xshift := 45;
           INSCTRL_yshift := 12;
           INSEDIT_yshift := 12;
           PATTORD_xshift := 0;
           MaxCol := MAX_COLUMNS;
           MaxLn := MAX_ROWS;
           hard_maxcol := MAX_COLUMNS;
           hard_maxln := 60;
           work_MaxCol := MAX_COLUMNS;
           work_MaxLn := 50;
           scr_font_width := 8;
           scr_font_height := 16;
         end;
      // 90x47
      4: begin
           SCREEN_RES_X := 800;
           SCREEN_RES_Y := 600;
           MAX_COLUMNS := 90;
           MAX_ROWS := 38;
           MAX_ORDER_COLS := 9;
           MAX_TRACKS := 5;
           MAX_PATTERN_ROWS := 16;
           INSCTRL_xshift := 0;
           INSCTRL_yshift := 4;
           INSEDIT_yshift := 12;
           PATTORD_xshift := 0;
           MaxCol := MAX_COLUMNS;
           MaxLn := MAX_ROWS;
           hard_maxcol := MAX_COLUMNS;
           hard_maxln := 38;
           work_MaxCol := MAX_COLUMNS;
           work_MaxLn := 38;
           scr_font_width := 8;
           scr_font_height := 16;
           GOTOXY_xshift := ((SCREEN_RES_X DIV scr_font_width)-MAX_COLUMNS) DIV 2;
         end;
      // 120x47
      5: begin
           SCREEN_RES_X := 1024;
           SCREEN_RES_Y := 768;
           MAX_COLUMNS := 120;
           MAX_ROWS := 48;
           MAX_ORDER_COLS := 13;
           MAX_TRACKS := 7;
           MAX_PATTERN_ROWS := 26;
           INSCTRL_xshift := 15;
           INSCTRL_yshift := 7;
           INSEDIT_yshift := 12;
           PATTORD_xshift := 1;
           MaxCol := MAX_COLUMNS;
           MaxLn := MAX_ROWS;
           hard_maxcol := MAX_COLUMNS;
           hard_maxln := 48;
           work_MaxCol := MAX_COLUMNS;
           work_MaxLn := 48;
           scr_font_width := 8;
           scr_font_height := 16;
           GOTOXY_xshift := ((SCREEN_RES_X DIV scr_font_width)-MAX_COLUMNS) DIV 2;
         end;
{$IFDEF GO32V2}
      // compatibility text-mode
      3: Case comp_text_mode of
           0,
           1: begin
                SCREEN_RES_X := 720;
                SCREEN_RES_Y := 480;
                MAX_COLUMNS := 90;
                MAX_ROWS := 40;
                MAX_ORDER_COLS := 9;
                MAX_TRACKS := 5;
                MAX_PATTERN_ROWS := 18;
                INSCTRL_xshift := 0;
                INSCTRL_yshift := 0;
                PATTORD_xshift := 0;
                INSEDIT_yshift := 0;
                MaxCol := MAX_COLUMNS;
                MaxLn := MAX_ROWS;
                hard_maxcol := MAX_COLUMNS;
                hard_maxln := 30;
                work_MaxCol := MAX_COLUMNS;
                work_MaxLn := 30;
                scr_font_width := 9;
                scr_font_height := 16;
              end;
         end;
    end
  else
    // VESA-emulated text-mode
    Case get_VESA_emulated_mode_idx of
      // 90x30 (default mode)
      0: begin
           SCREEN_RES_X := 800;
           SCREEN_RES_Y := 600;
           MAX_COLUMNS := 90;
           MAX_ROWS := 40;
           MAX_ORDER_COLS := 9;
           MAX_TRACKS := 5;
           MAX_PATTERN_ROWS := 18;
           INSCTRL_xshift := 0;
           INSCTRL_yshift := 0;
           INSEDIT_yshift := 0;
           PATTORD_xshift := 0;
           MaxCol := MAX_COLUMNS;
           MaxLn := MAX_ROWS;
           hard_maxcol := MAX_COLUMNS;
           hard_maxln := 30;
           work_MaxCol := MAX_COLUMNS;
           work_MaxLn := 30;
           scr_font_width := 8;
           scr_font_height := 16;
         end;
      // 90x47
      1: begin
           SCREEN_RES_X := 800;
           SCREEN_RES_Y := 600;
           MAX_COLUMNS := 90;
           MAX_ROWS := 46;
           MAX_ORDER_COLS := 9;
           MAX_TRACKS := 5;
           MAX_PATTERN_ROWS := 24;
           INSCTRL_xshift := 0;
           INSCTRL_yshift := 4;
           INSEDIT_yshift := 0;
           PATTORD_xshift := 0;
           MaxCol := MAX_COLUMNS;
           MaxLn := MAX_ROWS;
           hard_maxcol := MAX_COLUMNS;
           hard_maxln := 36;
           work_MaxCol := MAX_COLUMNS;
           work_MaxLn := 36;
           scr_font_width := 8;
           scr_font_height := 16;
         end;
      // 120x47
      2: begin
           SCREEN_RES_X := 1024;
           SCREEN_RES_Y := 768;
           MAX_COLUMNS := 120;
           MAX_ROWS := 46;
           MAX_ORDER_COLS := 13;
           MAX_TRACKS := 7;
           MAX_PATTERN_ROWS := 24;
           INSCTRL_xshift := 15;
           INSCTRL_yshift := 7;
           INSEDIT_yshift := 12;
           PATTORD_xshift := 1;
           MaxCol := MAX_COLUMNS;
           MaxLn := MAX_ROWS;
           hard_maxcol := MAX_COLUMNS;
           hard_maxln := 47;
           work_MaxCol := MAX_COLUMNS;
           work_MaxLn := 36;
           scr_font_width := 8;
           scr_font_height := 16;
         end;
    end;
{$ELSE}
    end;
{$ENDIF}

  FillWord(screen_ptr^,MAX_SCREEN_MEM_SIZE DIV 2,$0700);
  SCREEN_MEM_SIZE := (SCREEN_RES_X DIV scr_font_width)*MAX_ROWS*2;
  move_to_screen_routine := move2screen;

  If (command_typing = 0) then _pattedit_lastpos := 4*MAX_TRACKS
  else _pattedit_lastpos := 10*MAX_TRACKS;

  Case MAX_COLUMNS of
    120: temp := 1;
    180: temp := 2;
    else temp := 0;
  end;

  patt_win[1] := patt_win_tracks[temp][1];
  patt_win[2] := patt_win_tracks[temp][2];
  patt_win[3] := patt_win_tracks[temp][3];
  patt_win[4] := patt_win_tracks[temp][4];
  patt_win[5] := patt_win_tracks[temp][5];

  _dbg_leave; //EXIT //TxtScrIO_Init
end;
