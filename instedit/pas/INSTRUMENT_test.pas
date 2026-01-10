// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure INSTRUMENT_test(instr,instr2: Byte; chan: Byte; fkey: Word; process_macros: Boolean);

var
  temp,temp2,temp3,temp5: Byte;
  valid_key,temp4: Boolean;
  chan_handle: array[1..20] of Byte;
  channels: Byte;
  _1op_preview,_4op_mode: Boolean;

{$I instedit/_1op_preview_active.pas}
{$I instedit/output_note.pas}

begin { INSTRUMENT_test }
  _dbg_enter ({$I %FILE%}, 'INSTRUMENT_test');

  If ctrl_pressed or alt_pressed or shift_pressed then
    begin
      _dbg_leave; EXIT; //INSTRUMENT_test
    end;

  _1op_preview := _1op_preview_active;
  _4op_mode := NOT _1op_preview_active and (songdata.flag_4op <> 0) and (instr2 <> 0);

  valid_key := FALSE;
  For temp := 1 to 29 do
    If NOT shift_pressed then
      If (board_scancodes[temp] = HI(fkey)) then
        begin valid_key := TRUE; BREAK; end;

  If NOT valid_key or
     NOT (temp+12*(current_octave-1)-1 in [0..12*8+1]) then
    begin
      _dbg_leave; EXIT; //INSTRUMENT_test
    end;

  If (Empty(songdata.instr_data[instr],SizeOf(songdata.instr_data[instr])) and
      Empty(songdata.instr_macros[instr],SizeOf(songdata.instr_macros[instr]))) or
     (_4op_mode and Empty(songdata.instr_data[instr2],SizeOf(songdata.instr_data[instr2])) and
                    Empty(songdata.instr_macros[instr2],SizeOf(songdata.instr_macros[instr2]))) then
    begin
      _dbg_leave; EXIT; //INSTRUMENT_test
    end;

  temp2 := temp;
  ins_trailing_flag := TRUE;
  status_backup.replay_forbidden := replay_forbidden;
  status_backup.play_status := play_status;
  replay_forbidden := TRUE;
  If (play_status <> isStopped) then play_status := isPaused;
  nul_volume_bars;
  really_no_status_refresh := TRUE;

  Move(channel_flag,channel_flag_backup,SizeOf(channel_flag_backup));
  Move(event_table,event_table_backup,SizeOf(event_table_backup));
  common_flag_backup := songdata.common_flag;
  volume_scaling_backup := volume_scaling;
  songdata.common_flag := songdata.common_flag AND NOT $80;
  volume_scaling := FALSE;
  FillChar(channel_flag,SizeOf(channel_flag),BYTE(TRUE));
  Move(pan_lock,pan_lock_backup,SizeOf(pan_lock));
  Move(volume_lock,volume_lock_backup,SizeOf(volume_lock));
  Move(peak_lock,peak_lock_backup,SizeOf(volume_lock));
  Move(panning_table,panning_table_backup,SizeOf(panning_table));
  FillChar(pan_lock,SizeOf(pan_lock),0);
  FillChar(volume_lock,SizeOf(volume_lock),0);
  FillChar(peak_lock,SizeOf(volume_lock),0);
  flag_4op_backup := songdata.flag_4op;
  If NOT percussion_mode and
     NOT (songdata.flag_4op <> 0) then channels := 18
  else If NOT (songdata.flag_4op <> 0) then channels := 15
       else begin
              If _4op_mode then
                begin
                  songdata.flag_4op := $3f;
                  channels := 6;
                end
              else begin
                     songdata.flag_4op := 0;
                     If NOT percussion_mode then channels := 18
                     else channels := 15;
                   end;
            end;

  reset_player;
  FillChar(chan_handle,SizeOf(chan_handle),0);
  Move(fmpar_table,fmpar_table_backup,SizeOf(fmpar_table_backup));
  Move(volume_table,volume_table_backup,SizeOf(volume_table_backup));
  Move(freq_table,freq_table_backup,SizeOf(freq_table));
  Move(freqtable2,freqtable2_backup,SizeOf(freqtable2));
  Move(keyoff_loop,keyoff_loop_backup,SizeOf(keyoff_loop));
  FillChar(keyoff_loop,SizeOf(keyoff_loop),FALSE);

  misc_register := current_tremolo_depth SHL 7+
                   current_vibrato_depth SHL 6+
                   BYTE(percussion_mode) SHL 5;
  key_off(17);
  key_off(18);
  opl2out(_instr[11],misc_register);

  If percussion_mode and
     (songdata.instr_data[instr].perc_voice in [1..5]) then
    begin
      output_note(songdata.instr_data[instr].perc_voice+15,temp2);
      While scankey(board_scancodes[temp2]) do
        begin
{$IFDEF GO32V2}
          realtime_gfx_poll_proc;
{$ELSE}
          _draw_screen_without_delay := TRUE;
          keyboard_poll_input;
{$ENDIF}
         keyboard_reset_buffer;
         draw_screen;
        end;
    end
  else Repeat
{$IFNDEF GO32V2}
         keyboard_poll_input;
{$ENDIF}
         valid_key := FALSE;
         For temp := 1 to 29 do
           begin
             temp2 := board_scancodes[temp];
             temp4 := scankey(temp2);

             If NOT _4op_mode then
               begin
                 temp3 := get_chanpos(chan_handle,channels,temp2);
                 temp5 := get_chanpos(chan_handle,channels,0);
               end
             else begin
                    temp3 := get_chanpos2(chan_handle,channels,temp2);
                    temp5 := get_chanpos2(chan_handle,channels,0);
                  end;

             If temp4 then valid_key := TRUE;
             If temp4 and (temp3 = 0) and (temp5 <> 0) then
               output_note(temp5,temp);

             If NOT temp4 and (temp3 <> 0) then
               begin
                 chan_handle[temp3] := 0;
                 key_off(temp3);
                 If alt_pressed then keyoff_loop[temp3] := TRUE;
               end;
           end;
{$IFDEF GO32V2}
          realtime_gfx_poll_proc;
{$ELSE}
          _draw_screen_without_delay := TRUE;
          keyboard_poll_input;
{$ENDIF}
         keyboard_reset_buffer;
         draw_screen;
       until NOT valid_key;

  While ctrl_pressed do
    begin
{$IFDEF GO32V2}
      realtime_gfx_poll_proc;
{$ELSE}
      _draw_screen_without_delay := TRUE;
      keyboard_poll_input;
{$ENDIF}
      keyboard_reset_buffer;
      draw_screen;
    end;

  For temp := 1 to 20 do key_off(temp);
  songdata.flag_4op := flag_4op_backup;
  Move(fmpar_table_backup,fmpar_table,SizeOf(fmpar_table));
  Move(volume_table_backup,volume_table,SizeOf(volume_table));
  Move(panning_table_backup,panning_table,SizeOf(panning_table));
  songdata.common_flag := common_flag_backup;
  volume_scaling := volume_scaling_backup;
  If (status_backup.play_status = isPlaying) then reset_player;

  Move(channel_flag_backup,channel_flag,SizeOf(channel_flag));
  Move(event_table_backup,event_table,SizeOf(event_table));
  Move(pan_lock_backup,pan_lock,SizeOf(pan_lock));
  Move(volume_lock_backup,volume_lock,SizeOf(volume_lock));
  Move(peak_lock_backup,peak_lock,SizeOf(volume_lock));

  really_no_status_refresh := FALSE;
  Move(freq_table_backup,freq_table,SizeOf(freq_table));
  Move(freqtable2_backup,freqtable2,SizeOf(freqtable2));
  Move(keyoff_loop_backup,keyoff_loop,SizeOf(keyoff_loop));
  FillChar(macro_table,SizeOf(macro_table),0);
  replay_forbidden := status_backup.replay_forbidden;
  play_status := status_backup.play_status;
  ins_trailing_flag := FALSE;
  keyboard_reset_buffer;

  _dbg_leave; //EXIT //INSTRUMENT_test
end;
