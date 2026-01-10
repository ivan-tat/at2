// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function FILE_open(masks: String; loadBankPossible: Boolean): Byte;

var
  fname,temps: String;
  mpos,index: Byte;
  old_ext_proc: procedure; cdecl;
  old_songdata_source: String;
  old_play_status: tPLAY_STATUS;
  old_tracing: Boolean;
  temp_marks: array[1..255] of Char;
  temp_marks2: array[0..$7f] of Char;
  xstart,ystart: Byte;
  flag: Byte;

{$I adt2extn/pas/FILE_open_restore.pas}

label _jmp1;

begin
  _dbg_enter ({$I %FILE%}, 'FILE_open');

  flag := BYTE_NULL;
  old_play_status := play_status;
  old_tracing := tracing;
  If (Pos('a2i',Lower(masks)) = 0) and (Pos('a2f',Lower(masks)) = 0) and (Pos('a2w',Lower(masks)) = 0) then mpos := 1
  else mpos := 2;

  For index := 1 to 255 do
    temp_marks[index] := songdata.instr_names[index][1];

  For index := 0 to $7f do
    temp_marks2[index] := songdata.pattern_names[index][1];

_jmp1:
  If _force_program_quit then
    begin
      _dbg_leave; EXIT; //FILE_open
    end;

  old_songdata_source := songdata_source;
  If NOT quick_cmd then
    begin
      fs_environment.last_file := last_file[mpos];
      fs_environment.last_dir  := last_dir[mpos];

      old_ext_proc := mn_environment.ext_proc;
      If (mpos = 2) then mn_environment.ext_proc := fselect_external_proc;
      fname := Fselect(masks);
      mn_environment.ext_proc := old_ext_proc;

      last_file[mpos] := fs_environment.last_file;
      last_dir[mpos]  := fs_environment.last_dir;

      If (mn_environment.keystroke <> kENTER) then
        begin
          _dbg_leave; EXIT; //FILE_open
        end
      else If (mpos = 1) then songdata_source := fname
           else instdata_source := fname;
    end
  else If (mpos = 1) then fname := songdata_source
       else fname := instdata_source;

  load_flag := BYTE_NULL;
  limit_exceeded := FALSE;
  HideCursor;

  nul_volume_bars;
  no_status_refresh := TRUE;

  If (Lower(ExtOnly(fname)) = 'a2m') or (Lower(ExtOnly(fname)) = 'a2t') then
    begin
      ScreenMemCopy(screen_ptr,ptr_screen_backup);
      centered_frame_vdest := screen_ptr;

      temps := Upper(ExtOnly(fname))+' FiLE';
      centered_frame(xstart,ystart,43,3,' '+temps+' ',
                     dialog_background+dialog_border,
                     dialog_background+dialog_title,
                     frame_double);

      progress_xstart := xstart+2;
      progress_ystart := ystart+2;
      progress_num_steps := 1;
      progress_step := 1;

      If (Lower(ExtOnly(fname)) = 'a2m') then
        temps := 'MODULE'
      else temps := 'TiNY MODULE';

      ShowCStr(screen_ptr,xstart+2,ystart+1,
               'DECOMPRESSiNG '+temps+' DATA...',
               dialog_background+dialog_text,
               dialog_background+dialog_hi_text);
      show_progress(DWORD_NULL);
    end;

  If (Lower(ExtOnly(fname)) = 'a2m') then a2m_file_loader;
  If (Lower(ExtOnly(fname)) = 'a2t') then a2t_file_loader;
  If (Lower(ExtOnly(fname)) = 'a2p') then a2p_file_loader;
  If (Lower(ExtOnly(fname)) = 'amd') then amd_file_loader;
  If (Lower(ExtOnly(fname)) = 'cff') then cff_file_loader;
  If (Lower(ExtOnly(fname)) = 'dfm') then dfm_file_loader;
  If (Lower(ExtOnly(fname)) = 'fmk') then fmk_file_loader;
  If (Lower(ExtOnly(fname)) = 'hsc') then hsc_file_loader;
  If (Lower(ExtOnly(fname)) = 'mtk') then mtk_file_loader;
  If (Lower(ExtOnly(fname)) = 'rad') then rad_file_loader;
  If (Lower(ExtOnly(fname)) = 's3m') then s3m_file_loader;
  If (Lower(ExtOnly(fname)) = 'sat') then sat_file_loader;
  If (Lower(ExtOnly(fname)) = 'sa2') then sa2_file_loader;
  If (Lower(ExtOnly(fname)) = 'xms') then amd_file_loader;
  If (Lower(ExtOnly(fname)) = 'a2i') then a2i_file_loader;
  If (Lower(ExtOnly(fname)) = 'a2f') then a2f_file_loader;

  If ((Lower(ExtOnly(fname)) = 'a2m') or (Lower(ExtOnly(fname)) = 'a2t')) and
     (load_flag = 1)  then
    begin
      progress_num_steps := 1;
      progress_step := 1;
      progress_value := 1;
      progress_old_value := BYTE_NULL;
      _draw_screen_without_delay := TRUE;
      show_progress(1);
      // delay for awhile to show progress bar at 100%
{$IFDEF GO32V2}
      CRT.Delay(500);
{$ELSE}
      SDL_Delay(200);
{$ENDIF}
      _restore;
    end;

  If (Lower(ExtOnly(fname)) = 'a2b') then
    If shift_pressed and NOT ctrl_pressed and
       NOT alt_pressed then
      begin
        If loadBankPossible then
          begin
            index := Dialog('ALL UNSAVED INSTRUMENT DATA WiLL BE LOST$'+
                            'DO YOU WiSH TO CONTiNUE?$',
                            '~Y~UP$~N~OPE$',' A2B LOADER ',1);
            If (dl_environment.keystroke <> kESC) and (index = 1) then
              a2b_file_loader(FALSE,loadBankPossible); // w/o bank selector
          end
        else a2b_file_loader(TRUE,loadBankPossible); // w/ bank selector
      end
    else a2b_file_loader(TRUE,loadBankPossible); // w/ bank selector

  If (Lower(ExtOnly(fname)) = 'a2w') then
    If shift_pressed and NOT ctrl_pressed and
       NOT alt_pressed then
      begin
        If loadBankPossible then
          begin
            If _arp_vib_loader then
              index := Dialog('ALL UNSAVED ARPEGGiO/ViBRATO MACRO DATA WiLL BE LOST$'+
                              'DO YOU WiSH TO CONTiNUE?$',
                              '~Y~UP$~N~OPE$',' A2W LOADER ',1)
            else
              index := Dialog('ALL UNSAVED iNSTRUMENT AND MACRO DATA WiLL BE LOST$'+
                              'DO YOU WiSH TO CONTiNUE?$',
                              '~Y~UP$~N~OPE$',' A2W LOADER ',1);
            If (dl_environment.keystroke <> kESC) and (index = 1) then
              a2w_file_loader(TRUE,_arp_vib_loader,FALSE,loadBankPossible,FALSE); // w/o bank selector
          end
        else a2w_file_loader(TRUE,_arp_vib_loader,TRUE,loadBankPossible,FALSE); // w/ bank selector
        _arp_vib_loader := FALSE;
      end
    else
      begin
        a2w_file_loader(TRUE,_arp_vib_loader,TRUE,loadBankPossible,FALSE); // w/ bank selector
        _arp_vib_loader := FALSE;
      end;

  If (Lower(ExtOnly(fname)) = 'bnk') then bnk_file_loader;
  If (Lower(ExtOnly(fname)) = 'cif') then cif_file_loader;
  If (Lower(ExtOnly(fname)) = 'fib') then fib_file_loader;
  If (Lower(ExtOnly(fname)) = 'fin') then fin_file_loader;
  If (Lower(ExtOnly(fname)) = 'ibk') then ibk_file_loader;
  If (Lower(ExtOnly(fname)) = 'ins') then ins_file_loader;
  If (Lower(ExtOnly(fname)) = 'sbi') then sbi_file_loader;
  If (Lower(ExtOnly(fname)) = 'sgi') then sgi_file_loader;

//  If use_large_cursor then WideCursor
//  else ThinCursor;

  If (mpos = 1) then
    Case load_flag of
      0: If (old_songdata_source <> '') and
            (old_songdata_source <> songdata_source) then
           begin
             force_scrollbars := TRUE;
             PATTERN_ORDER_page_refresh(pattord_page);
             PATTERN_page_refresh(pattern_page);
             force_scrollbars := FALSE;

             index := Dialog('THERE WAS AN ERROR WHiLE LOADiNG NEW MODULE$'+
                             'DO YOU WiSH TO RELOAD PREViOUS?$',
                             '~Y~UP$~N~OPE$',' PATTERN EDiTOR ',1);

             If (dl_environment.keystroke <> kESC) and (index = 1) then
               begin
                 quick_cmd := TRUE;
                 songdata_source := old_songdata_source;
                 FILE_open('*.a2m$*.a2t$*.a2p$*.amd$*.cff$*.dfm$*.hsc$*.mtk$*.rad$'+
                           '*.s3m$*.sat$*.sa2$*.xms$',FALSE);
                 quick_cmd := FALSE;
               end;
           end
         else
           begin
             force_scrollbars := TRUE;
             PATTERN_ORDER_page_refresh(pattord_page);
             PATTERN_page_refresh(pattern_page);
             force_scrollbars := FALSE;

             index := Dialog('THERE WAS AN ERROR WHiLE LOADiNG NEW MODULE$'+
                             'DO YOU WiSH TO CONTiNUE?$',
                             '~Y~UP$~N~OPE$',' PATTERN EDiTOR ',1);

             If (dl_environment.keystroke <> kESC) and (index = 1) then
             else If (dl_environment.keystroke <> kESC) then init_songdata;
           end;

      1: begin
           If limit_exceeded then
             If (old_songdata_source <> '') and
                (old_songdata_source <> songdata_source) then
               begin
                 force_scrollbars := TRUE;
                 PATTERN_ORDER_page_refresh(pattord_page);
                 PATTERN_page_refresh(pattern_page);
                 force_scrollbars := FALSE;

                 index := Dialog('MODULE WAS NOT COMPLETELY LOADED DUE TO LACK OF MEMORY$'+
                                 'DO YOU WiSH TO RELOAD PREViOUS?$',
                                 '~Y~UP$~N~OPE$',' PATTERN EDiTOR ',1);

                 If (dl_environment.keystroke <> kESC) and (index = 1) then
                   begin
                     quick_cmd := TRUE;
                     songdata_source := old_songdata_source;
                     FILE_open('*.a2m$*.a2t$*.a2p$*.amd$*.cff$*.dfm$*.hsc$*.mtk$*.rad$'+
                               '*.s3m$*.sat$*.sa2$*.xms$',FALSE);
                     quick_cmd := FALSE;
                   end;
               end
             else
               begin
                 force_scrollbars := TRUE;
                 PATTERN_ORDER_page_refresh(pattord_page);
                 PATTERN_page_refresh(pattern_page);
                 force_scrollbars := FALSE;

                 index := Dialog('MODULE WAS NOT COMPLETELY LOADED DUE TO LACK OF MEMORY$'+
                                 'DO YOU WiSH TO CONTiNUE?$',
                                 '~Y~UP$~N~OPE$',' PATTERN EDiTOR ',1);

                 If (dl_environment.keystroke <> kESC) and (index = 1) then
                 else If (dl_environment.keystroke <> kESC) then init_songdata;
               end;

           speed_update    := BOOLEAN(songdata.common_flag AND 1);
           lockvol         := BOOLEAN(songdata.common_flag SHR 1 AND 1);
           lockVP          := BOOLEAN(songdata.common_flag SHR 2 AND 1);
           tremolo_depth   :=         songdata.common_flag SHR 3 AND 1;
           vibrato_depth   :=         songdata.common_flag SHR 4 AND 1;
           panlock         := BOOLEAN(songdata.common_flag SHR 5 AND 1);
           percussion_mode := BOOLEAN(songdata.common_flag SHR 6 AND 1);
           volume_scaling  := BOOLEAN(songdata.common_flag SHR 7 AND 1);

           mark_line := songdata.bpm_data.rows_per_beat;
           IRQ_freq_shift := songdata.bpm_data.tempo_finetune;

           current_tremolo_depth := tremolo_depth;
           current_vibrato_depth := vibrato_depth;

           If NOT percussion_mode then
             begin
               _chan_n := _chmm_n;
               _chan_m := _chmm_m;
               _chan_c := _chmm_c;
             end
           else
             begin
               _chan_n := _chpm_n;
               _chan_m := _chpm_m;
               _chan_c := _chpm_c;
             end;

           init_buffers;
           If (Lower(ExtOnly(fname)) <> 'a2p') then
             For index := 1 to 255 do
               songdata.instr_names[index] :=
                 ' iNS_'+byte2hex(index)+#247' '+
                 Copy(songdata.instr_names[index],10,32);

           If (Lower(ExtOnly(fname)) <> 'a2p') then
             For index := 0 to $7f do
               songdata.pattern_names[index] :=
                 ' PAT_'+byte2hex(index)+'  '#247' '+
                 Copy(songdata.pattern_names[index],12,30);

           If NOT quick_cmd then
             begin
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
               For index := 1 to 255 do
                 begin
                   add_bank_position('?instrument_editor?'+byte2hex(index)+'?carrier',-1,1);
                   add_bank_position('?instrument_editor?'+byte2hex(index)+'?carrier?hpos',-1,1);
                   add_bank_position('?instrument_editor?'+byte2hex(index)+'?carrier?vpos',-1,1);
                   add_bank_position('?instrument_editor?'+byte2hex(index)+'?modulator?hpos',-1,1);
                   add_bank_position('?instrument_editor?'+byte2hex(index)+'?modulator?vpos',-1,1);
                 end;
               For index := 1 to _rearrange_nm_tracks do _rearrange_tracklist_idx[index] := index;
               _rearrange_track_pos := 1;
             end
           else begin
                  For index := 1 to 255 do
                     songdata.instr_names[index][1] := temp_marks[index];
                  For index := 0 to $7f do
                     songdata.pattern_names[index][1] := temp_marks2[index];
                end;

           reset_player;
           If NOT quick_cmd or NOT keep_position then
              If (Lower(ExtOnly(fname)) <> 'a2p') and
                 NOT (shift_pressed and (mpos = 1) and (load_flag <> BYTE_NULL) and NOT quick_cmd and
                                        (old_play_status = isPlaying)) then
                POSITIONS_reset;
           songdata_crc := Update32(songdata,SizeOf(songdata),0);
           If (Lower(ExtOnly(fname)) <> 'a2p') then
             begin
               module_archived := TRUE;
               songdata_crc_ord := Update32(songdata.pattern_order,
                                            SizeOf(songdata.pattern_order),0);
             end;
         end
    end
  else
    If (load_flag <> BYTE_NULL) then module_archived := FALSE
    else If NOT quick_cmd then
           If (Lower(ExtOnly(fname)) = 'bnk') or
              (Lower(ExtOnly(fname)) = 'fib') or
              (Lower(ExtOnly(fname)) = 'ibk') then GOTO _jmp1;

  If (load_flag <> BYTE_NULL) then
    begin
      songdata.songname := FilterStr2(songdata.songname,_valid_characters,'_');
      songdata.composer := FilterStr2(songdata.composer,_valid_characters,'_');

      For index := 1 to 255 do
        songdata.instr_names[index] :=
          Copy(songdata.instr_names[index],1,9)+
          FilterStr2(Copy(cstr2str(songdata.instr_names[index]),10,32),_valid_characters,'_');
    end;

  If shift_pressed and
     (mpos = 1) and (load_flag <> BYTE_NULL) and NOT quick_cmd and
     (old_play_status = isPlaying) then
    begin
      start_playing;
      tracing := old_tracing;
      If NOT quick_cmd or NOT keep_position then
        If (Lower(ExtOnly(fname)) <> 'a2p') and NOT tracing then POSITIONS_reset;
    end
  else begin
         force_scrollbars := TRUE;
         PATTERN_ORDER_page_refresh(pattord_page);
         PATTERN_page_refresh(pattern_page);
         force_scrollbars := FALSE;
       end;

  flag := 1;
  FillChar(ai_table,SizeOf(ai_table),0);
  no_status_refresh := FALSE;
  FILE_open := flag;

  _dbg_leave; //EXIT //FILE_open
end;
