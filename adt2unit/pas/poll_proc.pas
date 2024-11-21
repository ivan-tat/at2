// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure poll_proc;

var
  temp: Byte;
{$IFNDEF ADT2PLAY}
  factor: Real;
{$ENDIF} // NOT DEFINED(ADT2PLAY)

begin
  _dbg_enter ({$I %FILE%}, 'poll_proc');

  If (NOT pattern_delay and (ticks-tick0+1 >= speed)) or
{$IFDEF ADT2PLAY}
     fast_forward then
{$ELSE} // NOT DEFINED(ADT2PLAY)
     fast_forward or _rewind or single_play then
{$ENDIF} // NOT DEFINED(ADT2PLAY)
    begin
{$IFNDEF ADT2PLAY}
      If debugging and
         NOT single_play and NOT pattern_break and
         (NOT space_pressed or no_step_debugging) then
        begin
          _dbg_leave; EXIT; //poll_proc
        end;
{$ENDIF} // NOT DEFINED(ADT2PLAY)

{$IFNDEF ADT2PLAY}
      If NOT single_play and
         NOT play_single_patt then
        begin
{$ENDIF} // NOT DEFINED(ADT2PLAY)
          If (songdata.pattern_order[current_order] > $7f) then
            If (calc_order_jump = -1) then
              begin
                _dbg_leave; EXIT; //poll_proc
              end;

          current_pattern := songdata.pattern_order[current_order];
{$IFNDEF ADT2PLAY}
        end;
{$ENDIF} // NOT DEFINED(ADT2PLAY)

      play_line;
{$IFDEF ADT2PLAY}
      If NOT fast_forward then
{$ELSE} // NOT DEFINED(ADT2PLAY)
      If NOT single_play and NOT (fast_forward or _rewind) then
{$ENDIF} // NOT DEFINED(ADT2PLAY)
        update_effects
      else For temp := 1 to speed do
             begin
               update_effects;
               If (temp MOD 4 = temp) then
                 update_extra_fine_effects;
               Inc(ticks);
             end;

{$IFDEF ADT2PLAY}
      pattern_break_docmd := pattern_break;
      pattern_break_oldord := current_order;
      If fast_forward or NOT pattern_delay then
{$ELSE} // NOT DEFINED(ADT2PLAY)
      tick0 := ticks;
      If NOT single_play and (fast_forward or _rewind or
                              NOT pattern_delay) then
{$ENDIF} // NOT DEFINED(ADT2PLAY)
        update_song_position;

{$IFDEF ADT2PLAY}
      If (pattern_break_docmd = TRUE) then
        pattern_break_loop := current_order = pattern_break_oldord;

      tick0 := ticks;
      If fast_forward then
{$ELSE} // NOT DEFINED(ADT2PLAY)
      If fast_forward or _rewind then
{$ENDIF} // NOT DEFINED(ADT2PLAY)
        If NOT pattern_delay then synchronize_song_timer;

{$IFDEF ADT2PLAY}
      If fast_forward and pattern_delay then
{$ELSE} // NOT DEFINED(ADT2PLAY)
      If (fast_forward or _rewind) and pattern_delay then
{$ENDIF} // NOT DEFINED(ADT2PLAY)
        begin
          tickD := 0;
          pattern_delay := FALSE;
        end;

{$IFNDEF ADT2PLAY}
      If fast_forward or _rewind then
        begin
{$IFDEF GO32V2}
          keyboard_reset_buffer;
{$ENDIF} // DEFINED(GO32V2)
          If fast_forward then factor := 1/(5-fforward_factor+1)
          else factor := 1/(5-rewind_factor+1);
          If (Abs(time_playing-time_playing0) > factor*(1+(1/255*tempo))/speed) then
            begin
              fast_forward := FALSE;
              _rewind := FALSE;
              time_playing0 := time_playing;
              synchronize_song_timer;
            end;
        end;
{$ENDIF} // NOT DEFINED(ADT2PLAY)
    end
  else
    begin
      update_effects;
      Inc(ticks);
{$IFNDEF ADT2PLAY}
      If NOT (debugging and NOT single_play and (NOT space_pressed or no_step_debugging)) then
{$ENDIF} // NOT DEFINED(ADT2PLAY)
        If pattern_delay and (tickD > 1) then Dec(tickD)
        else begin
{$IFDEF ADT2PLAY}
               If pattern_delay then
{$ELSE} // NOT DEFINED(ADT2PLAY)
               If pattern_delay and NOT single_play then
{$ENDIF} // NOT DEFINED(ADT2PLAY)
                 begin
                   tick0 := ticks;
                   update_song_position;
                 end;
               pattern_delay := FALSE;
             end;
    end;

  Inc(tickXF);
  If (tickXF MOD 4 = 0) then
    begin
      update_extra_fine_effects;
      Dec(tickXF,4);
    end;

  _dbg_leave; //EXIT //poll_proc
end;
