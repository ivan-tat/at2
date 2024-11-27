// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure synchronize_screen;
{$IFDEF GO32V2}
var
  regs: tRealRegs;
{$ENDIF}
begin
  _dbg_enter ({$I %FILE%}, 'synchronize_screen');

{$IFDEF GO32V2}
  If mouse_active then
    begin
      FillChar(regs,SizeOf(regs),0);
      regs.ax := $03;
      RealIntr($33,regs);
      scr_scroll_x := regs.cx;
      scr_scroll_y := regs.dx;
    end;

  If (scr_scroll_x <> old_scr_scroll_x) or
     (scr_scroll_y <> old_scr_scroll_y) then
    begin
      old_scr_scroll_x := scr_scroll_x;
      old_scr_scroll_y := scr_scroll_y;

      If (scr_scroll_x > scr_font_width*MaxCol-scr_font_width*hard_maxcol) then
        begin
          scr_scroll_x := scr_font_width*MaxCol-scr_font_width*hard_maxcol;
          update_mouse_position;
        end;

      If (scr_scroll_y > scr_font_height*MaxLn-scr_font_height*hard_maxln) then
        begin
          scr_scroll_y := scr_font_height*MaxLn-scr_font_height*hard_maxln;
          update_mouse_position;
        end;

      WaitRetrace;
      If NOT is_VESA_emulated_mode then
        SetTextDisp(scr_scroll_x,scr_scroll_y)
      else virtual_screen__first_row := scr_scroll_y*800;
    end;
{$ELSE}
  If NOT is_default_screen_mode then
    begin
      _dbg_leave; EXIT; //synchronize_screen
    end;
  If (screen_scroll_offset > 16*MaxLn-16*hard_maxln) then
    screen_scroll_offset := 16*MaxLn-16*hard_maxln;
  virtual_screen__first_row := screen_scroll_offset*SCREEN_RES_X;
{$ENDIF}

  _dbg_leave; //EXIT //synchronize_screen
end;
