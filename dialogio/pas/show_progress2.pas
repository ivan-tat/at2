// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure show_progress2(value,refresh_dif: Longint);
begin
  _dbg_enter ({$I %FILE%}, 'show_progress2');

  If (progress_num_steps = 0) or
     (progress_value = 0) then
    begin
      _dbg_leave; EXIT; //show_progress2
    end;
  If (value <> DWORD_NULL) then
    begin
      If (progress_num_steps = 1) then
        progress_new_value := Round(40/progress_value*value)
      else progress_new_value :=
             Round(40/progress_num_steps*PRED(progress_step)+
                   40/progress_num_steps/progress_value*value);
      progress_new_value := max(progress_new_value,40);
      If (Abs(progress_new_value-progress_old_value) >= refresh_dif) or
         (progress_new_value = 40) then
        begin
          progress_old_value := progress_new_value;
          ShowStr(screen_ptr,progress_xstart+35,progress_ystart-1,
                  ExpStrL(Num2Str(Round(100/40*progress_new_value),10)+'%',5,' '),
                  dialog_background+dialog_hi_text);
          ShowCStr(screen_ptr,
                   progress_xstart,progress_ystart,
                   '~'+ExpStrL('',progress_new_value,#219)+'~'+
                   ExpStrL('',40-progress_new_value,#219),
                   dialog_background+dialog_prog_bar1,
                   dialog_background+dialog_prog_bar2);
          realtime_gfx_poll_proc;
          draw_screen;
        end;
    end
  else begin
         ShowStr(screen_ptr,progress_xstart+35,progress_ystart-1,
                 ExpStrL('0%',5,' '),
                 dialog_background+dialog_hi_text);
         ShowStr(screen_ptr,
                 progress_xstart,progress_ystart,
                 ExpStrL('',40,#219),
                 dialog_background+dialog_prog_bar1);
         realtime_gfx_poll_proc;
         draw_screen;
       end;

  _dbg_leave; //EXIT //show_progress2
end;
