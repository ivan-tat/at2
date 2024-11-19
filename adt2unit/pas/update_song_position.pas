// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure update_song_position;

var
  temp: Byte;

begin
  _dbg_enter ({$I %FILE%}, 'update_song_position');

{$IFNDEF ADT2PLAY}
  For temp := 9 downto 2 do
    play_pos_buf[temp] := play_pos_buf[temp-1];
  play_pos_buf[1] := (current_pattern SHL 8)+current_line;

  If NOT _rewind then
    begin
{$ENDIF} // NOT DEFINED(ADT2PLAY)
      If (current_line < PRED(songdata.patt_len)) and NOT pattern_break then Inc(current_line)
      else begin
{$IFNDEF ADT2PLAY}
             If NOT (pattern_break and (next_line AND $0f0 = pattern_loop_flag)) and
                repeat_pattern then
               begin
                 FillChar(loopbck_table,SizeOf(loopbck_table),BYTE_NULL);
                 FillChar(loop_table,SizeOf(loop_table),BYTE_NULL);
                 current_line := 0;
                 pattern_break := FALSE;
               end
             else begin
{$ENDIF} // NOT DEFINED(ADT2PLAY)
                    If NOT (pattern_break and (next_line AND $0f0 = pattern_loop_flag)) and
                           (current_order < $7f) then
                      begin
                        FillChar(loopbck_table,SizeOf(loopbck_table),BYTE_NULL);
                        FillChar(loop_table,SizeOf(loop_table),BYTE_NULL);
                        Inc(current_order);
                      end;

                    If pattern_break and (next_line AND $0f0 = pattern_loop_flag) then
                      begin
                        temp := next_line-pattern_loop_flag;
                        next_line := loopbck_table[temp];
                        If (loop_table[temp][current_line] <> 0) then
                          Dec(loop_table[temp][current_line]);
                      end
                    else If pattern_break and (next_line AND $0f0 = pattern_break_flag) then
                           begin
                             If (event_table[next_line-pattern_break_flag].effect_def2 = ef_PositionJump) then
                               current_order := event_table[next_line-pattern_break_flag].effect2
                             else current_order := event_table[next_line-pattern_break_flag].effect;
                             pattern_break := FALSE;
                           end
                         else If (current_order > $7f) then
                                current_order := 0;

{$IFNDEF ADT2PLAY}
                    If NOT play_single_patt then
{$ENDIF} // NOT DEFINED(ADT2PLAY)
                      If (songdata.pattern_order[current_order] > $7f) then
                        If (calc_order_jump = -1) then
                          begin
                            _dbg_leave; EXIT; //update_song_position
                          end;

{$IFNDEF ADT2PLAY}
                    If NOT play_single_patt then
{$ENDIF} // NOT DEFINED(ADT2PLAY)
                      current_pattern := songdata.pattern_order[current_order];

                    If NOT pattern_break then current_line := 0
                    else begin
                           pattern_break := FALSE;
                           current_line := next_line;
                         end;
                  end;
{$IFNDEF ADT2PLAY}
           end;
    end
  else
    If (current_line > 0) then Dec(current_line);
{$ENDIF} // NOT DEFINED(ADT2PLAY)

  For temp := 1 to songdata.nm_tracks do
    begin
{$IFNDEF ADT2PLAY}
      ignore_note_once[temp] := FALSE;
{$ENDIF} // NOT DEFINED(ADT2PLAY)
      glfsld_table[temp] := 0;
      glfsld_table2[temp] := 0;
    end;

{$IFNDEF ADT2PLAY}
  If NOT play_single_patt then
{$ENDIF} // NOT DEFINED(ADT2PLAY)
    If (current_line = 0) and
       (current_order = calc_following_order(0)) and speed_update then
      begin
        tempo := songdata.tempo;
        speed := songdata.speed;
        update_timer(tempo);
      end;

  _dbg_leave; //EXIT //update_song_position
end;
