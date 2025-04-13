// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure draw_screen;
begin
  If _draw_screen_without_delay then
    _draw_screen_without_delay := FALSE
  else If do_synchronize and NOT (_screen_refresh_pending_frames > fps_down_factor) then
        EXIT //draw_screen
      else _screen_refresh_pending_frames := 0;
  If Compare(screen_ptr,ptr_screen_mirror,(SCREEN_RES_X DIV scr_font_width)*MAX_ROWS*2) then
    EXIT //draw_screen
  else begin
         ScreenMemCopy(screen_ptr,ptr_screen_mirror);
         If NOT is_VESA_emulated_mode then
           begin
             If NOT _draw_screen_without_vsync then
               WaitRetrace;
             If NOT (program_screen_mode in [4,5]) then
               dosmemput(v_seg,v_ofs,screen_ptr^,MAX_COLUMNS*MAX_ROWS*2)
             else begin
                    shift_text_screen;
                    dosmemput(v_seg,v_ofs,ptr_temp_screen2^,(SCREEN_RES_X DIV scr_font_width)*MAX_ROWS*2);
                  end;
           end;
       end;
  _draw_screen_without_vsync := FALSE;
  If is_VESA_emulated_mode then
    Case get_VESA_emulated_mode_idx of
      0: begin
           draw_VESA_screen_800x600_1;
           dump_VESA_buffer(800*600);
         end;

      1: begin
           draw_VESA_screen_800x600_2;
           dump_VESA_buffer(800*600);
         end;

      2: begin
           draw_VESA_screen_1024x768;
           dump_VESA_buffer(1024*768);
         end;
    end;

  //EXIT //draw_screen
end;
