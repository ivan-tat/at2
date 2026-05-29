// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure NUKE;

var
  temp,temp1,temp2: Byte;

begin
  _dbg_enter ({$I %FILE%}, 'NUKE');

  temp1 := Dialog('SO YOU THiNK iT REALLY SUCKS, DON''T YOU?$'+
                  'WHAT DO YOU WANT TO BE NUKED?$',
                  '~O~RDER$~P~ATTERNS$iNSTR [$~N~AMES$~R~EGS$~M~ACROS$]$ARP/~V~iB$~A~LL$',
                  ' NUKE''M ',clearpos);

  clearpos := temp1;
  If (dl_environment.keystroke <> kESC) then
    begin
      If (temp1 = 1) then
        begin
          FillChar(songdata.pattern_order,SizeOf(songdata.pattern_order),$080);
          PATTERN_ORDER_page_refresh(pattord_page);
        end;

      If (temp1 = 2) then
        begin
          For temp := 1 to max_patterns DIV 8 do
            FillChar(pattdata^[PRED(temp)],8*PATTERN_SIZE,0);
          PATTERN_ORDER_page_refresh(pattord_page);
          PATTERN_page_refresh(pattern_page);
          For temp2 := 0 to $7f do
            songdata.pattern_names[temp2] :=
              ' PAT_'+byte2hex(temp2)+'  '#247' ';
          pattern_list__page := 1;
        end;

      If (temp1 = 4) then
        begin
          For temp2 := 1 to 255 do
            begin
              FillChar(songdata.instr_names[temp2][2],SizeOf(songdata.instr_names[temp2])-2,0);
              songdata.instr_names[temp2] :=
                songdata.instr_names[temp2][1]+
                'iNS_'+byte2hex(temp2)+#247' ';
            end;
        end;

      If (temp1 = 5) then
        begin
          FillChar(songdata.instr_data,SizeOf(songdata.instr_data),0);
          FillChar(songdata.ins_4op_flags,SizeOf(songdata.ins_4op_flags),0);
          update_4op_flag_marks;
        end;

      If (temp1 = 6) then
        begin
          FillChar(songdata.instr_macros,SizeOf(songdata.instr_macros),0);
          _macro_editor__pos[FALSE] := 1;
          _macro_editor__fmreg_hpos[FALSE] := 1;
          _macro_editor__fmreg_hpos[TRUE] := 1;
          _macro_editor__fmreg_page[FALSE] := 1;
          _macro_editor__fmreg_page[TRUE] := 1;
          _macro_editor__fmreg_left_margin[FALSE] := 1;
          _macro_editor__fmreg_left_margin[TRUE] := 1;
          _macro_editor__fmreg_cursor_pos[FALSE] := 1;
          _macro_editor__fmreg_cursor_pos[TRUE] := 1;
          add_bank_position('?internal_instrument_data?macro?pos',-1,1);
          add_bank_position('?internal_instrument_data?macro?fmreg_page',-1,1);
          add_bank_position('?internal_instrument_data?macro?fmreg_hpos',-1,1);
          add_bank_position('?internal_instrument_data?macro?fmreg_vpos',-1,1);
          add_bank_position('?internal_instrument_data?macro?fmreg_left_margin',-1,1);
          add_bank_position('?internal_instrument_data?macro?fmreg_cursor_pos',-1,1);
        end;

      If (temp1 = 8) then
        begin
          FillChar(songdata.macro_table,SizeOf(songdata.macro_table),0);
          _macro_editor__pos[TRUE] := 8;
          _macro_editor__arpeggio_page[FALSE] := 1;
          _macro_editor__arpeggio_page[TRUE] := 1;
          _macro_editor__vibrato_hpos[FALSE] := 1;
          _macro_editor__vibrato_hpos[TRUE] := 1;
          _macro_editor__vibrato_page[FALSE] := 1;
          _macro_editor__vibrato_page[TRUE] := 1;
          add_bank_position('?internal_instrument_data?macro_av?pos',-1,1);
          add_bank_position('?internal_instrument_data?macro_av?arp_pos',-1,1);
          add_bank_position('?internal_instrument_data?macro_av?vib_pos',-1,1);
        end;

      If (temp1 = 9) then
        begin
          fade_out_playback(FALSE);
          stop_playing;
          tempo := init_tempo;
          speed := init_speed;
          mark_line := 4;
          IRQ_freq_shift := 0;
          playback_speed_shift := 0;
          init_songdata;
          POSITIONS_reset;
          songdata_title := 'noname.';
          FillChar(channel_flag,SizeOf(channel_flag),BYTE(TRUE));
          track_notes := FALSE;
          track_chan_start := 1;
          nm_track_chan := 1;
          remap_mtype := 1;
          remap_ins1 := 1;
          remap_ins2 := 1;
          remap_selection := 1;
          replace_selection := 1;
          replace_prompt := FALSE;
          replace_data.event_to_find.note := '???';
          replace_data.event_to_find.inst := '??';
          replace_data.event_to_find.fx_1 := '???';
          replace_data.event_to_find.fx_2 := '???';
          replace_data.new_event.note := '???';
          replace_data.new_event.inst := '??';
          replace_data.new_event.fx_1 := '???';
          replace_data.new_event.fx_2 := '???';
          current_inst := 1;
          pattern_list__page := 1;
          _macro_editor__pos[FALSE] := 1;
          _macro_editor__pos[TRUE] := 8;
          _macro_editor__fmreg_hpos[FALSE] := 1;
          _macro_editor__fmreg_hpos[TRUE] := 1;
          _macro_editor__fmreg_page[FALSE] := 1;
          _macro_editor__fmreg_page[TRUE] := 1;
          _macro_editor__fmreg_left_margin[FALSE] := 1;
          _macro_editor__fmreg_left_margin[TRUE] := 1;
          _macro_editor__fmreg_cursor_pos[FALSE] := 1;
          _macro_editor__fmreg_cursor_pos[TRUE] := 1;
          add_bank_position('?internal_instrument_data?macro?pos',-1,1);
          add_bank_position('?internal_instrument_data?macro?fmreg_page',-1,1);
          add_bank_position('?internal_instrument_data?macro?fmreg_hpos',-1,1);
          add_bank_position('?internal_instrument_data?macro?fmreg_vpos',-1,1);
          add_bank_position('?internal_instrument_data?macro?fmreg_left_margin',-1,1);
          add_bank_position('?internal_instrument_data?macro?fmreg_cursor_pos',-1,1);
          _macro_editor__arpeggio_page[FALSE] := 1;
          _macro_editor__arpeggio_page[TRUE] := 1;
          _macro_editor__vibrato_hpos[FALSE] := 1;
          _macro_editor__vibrato_hpos[TRUE] := 1;
          _macro_editor__vibrato_page[FALSE] := 1;
          _macro_editor__vibrato_page[TRUE] := 1;
          add_bank_position('?internal_instrument_data?macro_av?pos',-1,1);
          add_bank_position('?internal_instrument_data?macro_av?arp_pos',-1,1);
          add_bank_position('?internal_instrument_data?macro_av?vib_pos',-1,1);
          add_bank_position('?song_variables_window?pos',-1,1);
          add_bank_position('?song_variables_window?pos_4op',-1,0);
          add_bank_position('?replace_window?pos',-1,1);
          add_bank_position('?replace_window?posfx',-1,1);
          add_bank_position('?message_board?hpos',-1,1);
          add_bank_position('?message_board?vpos',-1,1);
          For temp := 1 to 255 do
            begin
              add_bank_position('?instrument_editor?'+byte2hex(temp)+'?carrier',-1,1);
              add_bank_position('?instrument_editor?'+byte2hex(temp)+'?carrier?hpos',-1,1);
              add_bank_position('?instrument_editor?'+byte2hex(temp)+'?carrier?vpos',-1,1);
              add_bank_position('?instrument_editor?'+byte2hex(temp)+'?modulator?hpos',-1,1);
              add_bank_position('?instrument_editor?'+byte2hex(temp)+'?modulator?vpos',-1,1);
            end;
          For temp := 1 to _rearrange_nm_tracks do _rearrange_tracklist_idx[temp] := temp;
          _rearrange_track_pos := 1;
        end
      else module_archived := FALSE;
    end;

  _dbg_leave; //EXIT //NUKE
end;
