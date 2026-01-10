// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure test_instrument_alt(chan: Byte; fkey: Word; loadMacros: Boolean; bankSelector: Boolean; loadArpVib: Boolean;
                              test_ins1,test_ins2: Byte);

{$I adt2ext3/pas/test_instrument_alt_output_note.pas}

var
  temp,temp2,temp3,temp5: Byte;
  valid_key,temp4: Boolean;
  temps: String;
  channels: Byte;

begin { test_instrument_alt }
  _dbg_enter ({$I %FILE%}, 'test_instrument_alt');

  valid_key := FALSE;
  For temp := 1 to 29 do
    If NOT shift_pressed then
      If (board_scancodes[temp] = HI(fkey)) then
        begin
          valid_key := TRUE;
          BREAK;
        end;

  If scankey(SC_F7) then
    For temp := 1 to 20 do reset_chan_data(temp);

  If NOT valid_key or
     NOT (temp+12*(current_octave-1)-1 in [0..12*8+1]) then
    begin
      _dbg_leave; EXIT; //test_instrument_alt
    end;

  If NOT percussion_mode then channels := 18
  else channels := 15;

  FillChar(temp_instrument,SizeOf(temp_instrument),0);
  FillChar(temp_instrument2,SizeOf(temp_instrument2),0);
  temp2 := temp;

  If bankSelector or loadArpVib then
    If NOT loadArpVib then
      begin
        temps := '';
        load_flag_alt := BYTE_NULL;
        If (test_ins2 = 0) then
          begin
            Move(temp_songdata.instr_data[test_ins1],
                 temp_instrument,
                 SizeOf(temp_instrument));
            Move(temp_songdata.instr_macros[test_ins1],
                 temp_instrument_macro,
                 SizeOf(temp_instrument_macro));
            Move(temp_songdata.dis_fmreg_col[test_ins1],
                 temp_instrument_dis_fmreg_col,
                 SizeOf(temp_instrument_dis_fmreg_col));
          end
        else begin
               Move(temp_songdata.instr_data[test_ins1],
                    temp_instrument,
                    SizeOf(temp_instrument));
               Move(temp_songdata.instr_macros[test_ins1],
                    temp_instrument_macro,
                    SizeOf(temp_instrument_macro));
               Move(temp_songdata.dis_fmreg_col[test_ins1],
                    temp_instrument_dis_fmreg_col,
                    SizeOf(temp_instrument_dis_fmreg_col));
               Move(temp_songdata.instr_data[test_ins2],
                    temp_instrument2,
                    SizeOf(temp_instrument2));
               Move(temp_songdata.instr_macros[test_ins2],
                    temp_instrument_macro2,
                    SizeOf(temp_instrument_macro2));
               Move(temp_songdata.dis_fmreg_col[test_ins2],
                    temp_instrument_dis_fmreg_col2,
                    SizeOf(temp_instrument_dis_fmreg_col2));
             end;
      end
    else
      begin
        load_flag_alt := BYTE_NULL;
        If (test_ins2 = 0) then
          begin
            Move(songdata.instr_data[test_ins1],
                 temp_instrument,
                 SizeOf(temp_instrument));
            Move(songdata.instr_macros[test_ins1],
                 temp_instrument_macro,
                 SizeOf(temp_instrument_macro));
            Move(songdata.dis_fmreg_col[test_ins1],
                 temp_instrument_dis_fmreg_col,
                 SizeOf(temp_instrument_dis_fmreg_col));
          end
        else begin
               Move(songdata.instr_data[test_ins1],
                    temp_instrument,
                    SizeOf(temp_instrument));
               Move(songdata.instr_macros[test_ins1],
                    temp_instrument_macro,
                    SizeOf(temp_instrument_macro));
               Move(songdata.dis_fmreg_col[test_ins1],
                    temp_instrument_dis_fmreg_col,
                    SizeOf(temp_instrument_dis_fmreg_col));
               Move(songdata.instr_data[test_ins2],
                    temp_instrument2,
                    SizeOf(temp_instrument2));
               Move(songdata.instr_macros[test_ins2],
                    temp_instrument_macro2,
                    SizeOf(temp_instrument_macro2));
               Move(songdata.dis_fmreg_col[test_ins2],
                    temp_instrument_dis_fmreg_col2,
                    SizeOf(temp_instrument_dis_fmreg_col2));
             end;
      end
  else
    begin
      temps := mn_environment.curr_item;
      load_flag_alt := BYTE_NULL;
{$IFDEF GO32V2}
      keyboard_toggle_sleep;
{$ENDIF}
      If (Lower(ExtOnly(temps)) = 'a2i') then
        a2i_file_loader_alt(temps)
      else If (Lower(ExtOnly(temps)) = 'a2f') then
             a2f_file_loader_alt(temps)
           else If (Lower(ExtOnly(temps)) = 'cif') then
                  cif_file_loader_alt(temps)
                else If (Lower(ExtOnly(temps)) = 'fin') then
                       fin_file_loader_alt(temps)
                     else If (Lower(ExtOnly(temps)) = 'ins') then
                            ins_file_loader_alt(temps)
                           else If (Lower(ExtOnly(temps)) = 'sbi') then
                                  sbi_file_loader_alt(temps)
                                 else If (Lower(ExtOnly(temps)) = 'sgi') then
                                        sgi_file_loader_alt(temps);
{$IFDEF GO32V2}
      keyboard_toggle_sleep;
{$ENDIF}
      If (load_flag_alt = BYTE_NULL) then
        begin
          _dbg_leave; EXIT; //test_instrument_alt
        end;
    end;

  If (test_ins2 <> 0) and Empty(temp_instrument2,SizeOf(temp_instrument2)) and
                          Empty(temp_instrument_macro2,SizeOf(temp_instrument_macro2)) then
    test_ins2 := 0;

  Move(songdata.macro_table,
       arp_vib_macro_table_backup,
       SizeOf(arp_vib_macro_table_backup));

  If loadArpVib then
    begin
      If arp_tab_selected then
        begin
          temp_instrument_macro.arpeggio_table := arpvib_arpeggio_table;
          Move(temp_songdata.macro_table[arpvib_arpeggio_table].arpeggio,
               songdata.macro_table[arpvib_arpeggio_table].arpeggio,
               SizeOf(songdata.macro_table[arpvib_arpeggio_table]));
        end
      else temp_instrument_macro.arpeggio_table := 0;
      If vib_tab_selected then
        begin
          temp_instrument_macro.vibrato_table := arpvib_vibrato_table;
          Move(temp_songdata.macro_table[arpvib_vibrato_table].vibrato,
               songdata.macro_table[arpvib_vibrato_table].vibrato,
               SizeOf(songdata.macro_table[arpvib_vibrato_table]));
        end
      else temp_instrument_macro.vibrato_table := 0;
      If (test_ins2 = 0) then
        Move(temp_instrument_macro,
             songdata.instr_macros[test_ins1],
             SizeOf(songdata.instr_macros[test_ins1]))
      else begin
             Move(temp_instrument_macro,
                  songdata.instr_macros[test_ins1],
                  SizeOf(songdata.instr_macros[test_ins1]));
             Move(temp_instrument_macro2,
                  songdata.instr_macros[test_ins2],
                  SizeOf(songdata.instr_macros[test_ins2]));
           end;
    end
  else If bankSelector and loadMacros then
         begin
           If (temp_instrument_macro.arpeggio_table <> 0) then
             Move(temp_songdata.macro_table[temp_instrument_macro.arpeggio_table].arpeggio,
                  songdata.macro_table[temp_instrument_macro.arpeggio_table].arpeggio,
                  SizeOf(songdata.macro_table[temp_instrument_macro.arpeggio_table]));
           If (temp_instrument_macro.vibrato_table <> 0) then
             Move(temp_songdata.macro_table[temp_instrument_macro.vibrato_table].vibrato,
                  songdata.macro_table[temp_instrument_macro.vibrato_table].vibrato,
                  SizeOf(songdata.macro_table[temp_instrument_macro.vibrato_table]));
         end;

  If (Empty(temp_instrument,SizeOf(temp_instrument)) and
      Empty(temp_instrument_macro,SizeOf(temp_instrument_macro))) or
     ((test_ins2 <> 0) and Empty(temp_instrument2,SizeOf(temp_instrument2)) and
                           Empty(temp_instrument_macro2,SizeOf(temp_instrument_macro2))) then
    begin
      _dbg_leave; EXIT; //test_instrument_alt
    end;

  ins_trailing_flag := TRUE;
  status_backup.replay_forbidden := replay_forbidden;
  status_backup.play_status := play_status;
  replay_forbidden := TRUE;
  If (play_status <> isStopped) then play_status := isPaused;
  nul_volume_bars;
  really_no_status_refresh := TRUE;
  reset_player;

  If (Lower(ExtOnly(temps)) = 'a2f') or loadMacros or loadArpVib then
    If (test_ins2 = 0) then
      begin
        Move(songdata.instr_macros[test_ins1],
             ins__fmreg_table_backup,
             SizeOf(ins__fmreg_table_backup));
        Move(songdata.dis_fmreg_col[test_ins1],
             ins__dis_fmreg_col_backup,
             SizeOf(ins__dis_fmreg_col_backup));
        Move(temp_instrument_macro,
             songdata.instr_macros[test_ins1],
             SizeOf(songdata.instr_macros[test_ins1]));
        Move(temp_instrument_dis_fmreg_col,
             songdata.dis_fmreg_col[test_ins1],
             SizeOf(songdata.dis_fmreg_col[test_ins1]));
      end
    else begin
           Move(songdata.instr_macros[test_ins1],
                ins__fmreg_table_backup,
                SizeOf(ins__fmreg_table_backup));
           Move(songdata.dis_fmreg_col[test_ins1],
                ins__dis_fmreg_col_backup,
                SizeOf(ins__dis_fmreg_col_backup));
           Move(temp_instrument_macro,
                songdata.instr_macros[test_ins1],
                SizeOf(songdata.instr_macros[test_ins1]));
           Move(temp_instrument_dis_fmreg_col,
                songdata.dis_fmreg_col[test_ins1],
                SizeOf(songdata.dis_fmreg_col[test_ins1]));
           Move(songdata.instr_macros[test_ins2],
                ins__fmreg_table_backup2,
                SizeOf(ins__fmreg_table_backup));
           Move(songdata.dis_fmreg_col[test_ins2],
                ins__dis_fmreg_col_backup2,
                SizeOf(ins__dis_fmreg_col_backup));
           Move(temp_instrument_macro2,
                songdata.instr_macros[test_ins2],
                SizeOf(songdata.instr_macros[test_ins2]));
           Move(temp_instrument_dis_fmreg_col2,
                songdata.dis_fmreg_col[test_ins2],
                SizeOf(songdata.dis_fmreg_col[test_ins2]));
         end
  else
    begin
      Move(songdata.instr_macros[test_ins1],
           ins__fmreg_table_backup,
           SizeOf(ins__fmreg_table_backup));
      Move(songdata.dis_fmreg_col[test_ins1],
           ins__dis_fmreg_col_backup,
           SizeOf(ins__dis_fmreg_col_backup));
      FillChar(songdata.instr_macros[test_ins1],SizeOf(songdata.instr_macros[test_ins1]),0);
      FillChar(songdata.dis_fmreg_col[test_ins1],SizeOf(songdata.dis_fmreg_col[test_ins1]),0);

          If (test_ins2 <> 0) then
        begin
          Move(songdata.instr_macros[test_ins2],
               ins__fmreg_table_backup2,
               SizeOf(ins__fmreg_table_backup));
          Move(songdata.dis_fmreg_col[test_ins2],
               ins__dis_fmreg_col_backup2,
               SizeOf(ins__dis_fmreg_col_backup));
          FillChar(songdata.instr_macros[test_ins2],SizeOf(songdata.instr_macros[test_ins2]),0);
          FillChar(songdata.dis_fmreg_col[test_ins2],SizeOf(songdata.dis_fmreg_col[test_ins2]),0);
        end;
    end;

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
       else If (test_ins2 <> 0) then
              begin
                songdata.flag_4op := $3f;
                channels := 6;
              end
            else begin
                   songdata.flag_4op := 0;
                   If NOT percussion_mode then channels := 18
                   else channels := 15;
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
     (temp_instrument.perc_voice in [1..5]) then
    begin
      output_note(temp_instrument.perc_voice+15,temp2);
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
         valid_key := FALSE;
         For temp := 1 to 29 do
           begin
             temp2 := board_scancodes[temp];
             temp4 := scankey(temp2);

             If NOT ((songdata.flag_4op <> 0) and (test_ins2 <> 0)) then
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

  Move(ins__fmreg_table_backup,
       songdata.instr_macros[test_ins1],
       SizeOf(songdata.instr_macros[test_ins1]));
  Move(ins__dis_fmreg_col_backup,
       songdata.dis_fmreg_col[test_ins1],
       SizeOf(songdata.dis_fmreg_col[test_ins1]));

  If (test_ins2 <> 0) then
    begin
      Move(ins__fmreg_table_backup,
           songdata.instr_macros[test_ins1],
           SizeOf(songdata.instr_macros[test_ins1]));
      Move(ins__dis_fmreg_col_backup,
           songdata.dis_fmreg_col[test_ins1],
           SizeOf(songdata.dis_fmreg_col[test_ins1]));
      Move(ins__fmreg_table_backup2,
           songdata.instr_macros[test_ins2],
           SizeOf(songdata.instr_macros[test_ins2]));
      Move(ins__dis_fmreg_col_backup2,
           songdata.dis_fmreg_col[test_ins2],
           SizeOf(songdata.dis_fmreg_col[test_ins2]));
    end;

  Move(arp_vib_macro_table_backup,
       songdata.macro_table,
       SizeOf(songdata.macro_table));

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

  _dbg_leave; //EXIT //test_instrument_alt
end;
