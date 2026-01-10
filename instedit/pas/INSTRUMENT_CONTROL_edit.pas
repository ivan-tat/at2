// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure INSTRUMENT_CONTROL_edit;

{$I instedit/min0.pas}
{$I instedit/_ftune.pas}
{$I instedit/_check_hpos_idx_l.pas}
{$I instedit/_check_hpos_idx_r.pas}
{$I instedit/_set_operator_flag.pas}

const
  KSL_volume:    array[0..3] of Byte = (0,$80,$40,$0c0);
  AM_Vibrato_EG: array[0..3] of Byte = ($80,$40,$10,$20);
  panning_pos:   array[0..2] of Byte = (1,0,2);

label _jmp1,_end;

begin { INSTRUMENT_CONTROL_edit }
  _dbg_enter ({$I %FILE%}, 'INSTRUMENT_CONTROL_edit');

  inst := Addr(songdata.instr_data[instrum_page]);
  FillChar(_operator_enabled,SizeOf(_operator_enabled),TRUE);
  If NOT (inst^.perc_voice in [2..5]) then
    If remember_ins_pos then
      begin
        temp := get_bank_position('?instrument_editor?'+byte2hex(instrum_page)+'?carrier',-1);
        If (temp <> 2) then carrier := TRUE
        else carrier := FALSE;
      end
    else carrier := TRUE
  else carrier := FALSE;

  If NOT remember_ins_pos then
    begin
      hpos := 1;
      vpos := 1;
    end
  else
    If carrier then
      begin
        hpos := min(get_bank_position('?instrument_editor?'+byte2hex(instrum_page)+'?carrier?hpos',-1),1);
        vpos := min(get_bank_position('?instrument_editor?'+byte2hex(instrum_page)+'?carrier?vpos',-1),1);
      end
    else begin
           hpos := min(get_bank_position('?instrument_editor?'+byte2hex(instrum_page)+'?modulator?hpos',-1),1);
           vpos := min(get_bank_position('?instrument_editor?'+byte2hex(instrum_page)+'?modulator?vpos',-1),1);
         end;
_jmp1:
  If _force_program_quit then
    begin
      _dbg_leave; EXIT; //INSTRUMENT_CONTROL_edit
    end;

  ScreenMemCopy(screen_ptr,ptr_screen_backup);
  HideCursor;

  ScreenMemCopy(screen_ptr,ptr_temp_screen);
  centered_frame_vdest := ptr_temp_screen;
  centered_frame(xstart,ystart,71,24+INSEDIT_yshift,' iNSTRUMENT EDiTOR (iNS_  ) ',
                 instrument_bckg+instrument_border,
                 instrument_bckg+instrument_title,
                 frame_double);
  centered_frame_vdest := screen_ptr;

  move_to_screen_data := ptr_temp_screen;
  move_to_screen_area[1] := xstart;
  move_to_screen_area[2] := ystart;
  move_to_screen_area[3] := xstart+71+2;
  move_to_screen_area[4] := ystart+24+INSEDIT_yshift+1;
  move2screen_alt;

  move_to_screen_area[1] := xstart+1;
  move_to_screen_area[2] := ystart;
  move_to_screen_area[3] := xstart+70;
  move_to_screen_area[4] := ystart+24+INSEDIT_yshift;
  Inc(xstart);

  _page_build;
  _sync_radio_buttons;

  If NOT _force_program_quit then
    Repeat
      keyboard_poll_input;
      _page_build;
      _page_refresh;
      If ((vpos in [1,2,4,5,7]) and (hpos = 1)) or
         ((vpos = 12) and (hpos = 3)) or
         ((vpos = 16) and (hpos = 2)) then
        With songdata.instr_data[current_inst] do
          begin
            Case vpos of
              1: If carrier then
                   GotoXY(xstart+inst_hpos[vpos,hpos]+
                          min0(fm_data.ATTCK_DEC_carrier SHR 4-1),
                          ystart+inst_vpos[vpos])
                 else
                   GotoXY(xstart+inst_hpos[vpos,hpos]+
                          min0(fm_data.ATTCK_DEC_modulator SHR 4-1),
                          ystart+inst_vpos[vpos]);

              2: If carrier then
                   GotoXY(xstart+inst_hpos[vpos,hpos]+
                          min0(fm_data.ATTCK_DEC_carrier AND $0f-1),
                          ystart+inst_vpos[vpos])
                 else
                   GotoXY(xstart+inst_hpos[vpos,hpos]+
                          min0(fm_data.ATTCK_DEC_modulator AND $0f-1),
                          ystart+inst_vpos[vpos]);

              4: If carrier then
                   GotoXY(xstart+inst_hpos[vpos,hpos]+
                          min0(fm_data.SUSTN_REL_carrier SHR 4-1),
                          ystart+inst_vpos[vpos])
                 else
                   GotoXY(xstart+inst_hpos[vpos,hpos]+
                          min0(fm_data.SUSTN_REL_modulator SHR 4-1),
                          ystart+inst_vpos[vpos]);

              5: If carrier then
                   GotoXY(xstart+inst_hpos[vpos,hpos]+
                          min0(fm_data.SUSTN_REL_carrier AND $0f-1),
                          ystart+inst_vpos[vpos])
                 else
                   GotoXY(xstart+inst_hpos[vpos,hpos]+
                          min0(fm_data.SUSTN_REL_modulator AND $0f-1),
                          ystart+inst_vpos[vpos]);

              7: If carrier then
                   GotoXY(xstart+inst_hpos[vpos,hpos]+
                          min0((63-fm_data.KSL_VOLUM_carrier AND $3f) DIV 2-1),
                          ystart+inst_vpos[vpos])
                 else
                   GotoXY(xstart+inst_hpos[vpos,hpos]+
                          min0((63-fm_data.KSL_VOLUM_modulator AND $3f) DIV 2-1),
                          ystart+inst_vpos[vpos]);

             12: GotoXY(xstart+inst_hpos[vpos,hpos]+
                        min0(ROUND(_get_feedback_val*(17/7))-1),
                        ystart+inst_vpos[vpos]);

             16: If (_get_finetune_val > 0) then
                   GotoXY(xstart+inst_hpos[vpos,hpos]+7+_ftune(_get_finetune_val)+1,
                          ystart+inst_vpos[vpos])
                 else If (_get_finetune_val < 0) then
                        GotoXY(xstart+inst_hpos[vpos,hpos]+1+7+_ftune(_get_finetune_val)-1,
                               ystart+inst_vpos[vpos])
                      else
                        GotoXY(xstart+inst_hpos[vpos,hpos]+1+7+_ftune(_get_finetune_val),
                               ystart+inst_vpos[vpos]);
            end;
          SetCursorShape($010c);
        end
      else
        begin
          If use_large_cursor then WideCursor
          else ThinCursor;
          GotoXY(xstart+inst_hpos[vpos,hpos],ystart+inst_vpos[vpos]);
        end;

      nope := FALSE;
      If keypressed then fkey := GetKey else GOTO _end;
      If NOT shift_pressed and NOT alt_pressed and NOT ctrl_pressed then
        If (get_4op_to_test <> 0) then
          INSTRUMENT_test(LO(get_4op_to_test),HI(get_4op_to_test),count_channel(pattern_hpos),fkey,TRUE)
        else INSTRUMENT_test(instrum_page,0,count_channel(pattern_hpos),fkey,TRUE);

      Case fkey of
        kAlt0:   FillChar(_operator_enabled,SizeOf(_operator_enabled),TRUE);
        kAlt1:   If shift_pressed then
                   _set_operator_flag(1,TRUE)
                 else _set_operator_flag(1,FALSE);
        kAlt2:   If shift_pressed then
                   _set_operator_flag(2,TRUE)
                 else _set_operator_flag(2,FALSE);
        kAlt3:   If shift_pressed then
                   _set_operator_flag(3,TRUE)
                 else _set_operator_flag(3,FALSE);
        kAlt4:   If shift_pressed then
                   _set_operator_flag(4,TRUE)
                 else _set_operator_flag(4,FALSE);

        kAltA:   begin hpos := 1; vpos := 1; end;
        kAltD:   begin hpos := 1; vpos := 2; end;
        kAltS:   begin hpos := 1; vpos := 4; end;
        kAltR:   begin hpos := 1; vpos := 5; end;
        kAltO:   begin hpos := 1; vpos := 7; end;

        kAltW:   begin
                   If carrier then vpos := 2+inst^.fm_data.WAVEFORM_carrier
                   else vpos := 2+inst^.fm_data.WAVEFORM_modulator;
                   If (vpos in [2,4,5,7]) then hpos := 2 else hpos := 1;
                 end;

        kAltK:   begin
                   If carrier then temp := inst^.fm_data.KSL_VOLUM_carrier
                   else temp := inst^.fm_data.KSL_VOLUM_modulator;
                   temp := temp SHR 6;
                   temp := temp SHR 1+temp AND 1 SHL 1;
                   vpos := 10+temp;
                   hpos := 1;
                 end;

        kAltP:   Case inst^.panning of
                   1: begin vpos := 17; hpos := 1; end;
                   0: begin vpos := 17; hpos := 2; end;
                   2: begin vpos := 17; hpos := 3; end;
                 end;

        kAltC:   If NOT (inst^.perc_voice in [2..5]) then
                   Case Pos(#11,_curr_connection_str) of
                     1: begin vpos := 12; hpos := 2; end;
                     2: begin vpos := 13; hpos := 2; end;
                     3: begin vpos := 14; hpos := 1; end;
                     4: begin vpos := 15; hpos := 1; end;
                     5: begin vpos := 16; hpos := 1; end;
                     6: begin vpos := 17; hpos := 4; end;
                   end;

        kAltF:   If NOT (inst^.perc_voice in [2..5]) then
                   begin vpos := 12; hpos := 3; end;

        kAltT:   begin vpos := 16; hpos := 2; end;
        kAltE:   begin vpos := 18; hpos := 1; end;

        kAltM:   begin
                   If carrier then temp := inst^.fm_data.AM_VIB_EG_carrier AND $0f
                   else temp := inst^.fm_data.AM_VIB_EG_modulator AND $0f;
                   hpos := 2+(temp MOD 6);
                   vpos := 19+(temp DIV 6);
                 end;

        kLEFT:   If (hpos-1 >= 1) and (inst_hpos[vpos,hpos-1] <> 0) then Dec(hpos)
                 else If (hpos-2 >= 1) and (inst_hpos[vpos,hpos-2] <> 0) then Dec(hpos,2)
                      else If (vpos > 1) then
                             begin
                               Repeat Dec(vpos) until (_check_hpos_idx_r(vpos) <> 0);
                               hpos := _check_hpos_idx_r(vpos);
                             end
                           else
                             begin
                               vpos := inst_vpos_max; hpos := inst_hpos_max;
                               While (hpos > 1) and (inst_hpos[vpos,hpos] = 0) do Dec(hpos);
                             end;

        kRIGHT:  If (hpos+1 <= inst_hpos_max) and (inst_hpos[vpos,hpos+1] <> 0) then Inc(hpos)
                 else If (hpos+2 <= inst_hpos_max) and (inst_hpos[vpos,hpos+2] <> 0) then Inc(hpos,2)
                      else If (vpos < inst_vpos_max) then
                             begin
                               Repeat Inc(vpos) until (_check_hpos_idx_l(vpos) <> 0);
                               hpos := _check_hpos_idx_l(vpos);
                             end
                           else begin vpos := 1; hpos := 1; end;

        kUP:          If (vpos > 1) then
                        begin
                          Repeat Dec(vpos) until (inst_hpos[vpos,1] <> 0);
                     temp := hpos; hpos := inst_hpos_max;
                     While (hpos > 1) and ((inst_hpos[vpos,hpos] = 0) or
                       (inst_hpos[vpos,hpos] > inst_hpos[vpos+1,temp])) do Dec(hpos);
                   end
                 else
                   begin
                     vpos := inst_vpos_max; temp := hpos; hpos := inst_hpos_max;
                     While (hpos > 1) and ((inst_hpos[vpos,hpos] = 0) or
                       (inst_hpos[vpos,hpos] > inst_hpos[vpos+1,temp])) do Dec(hpos);
                   end;

        kDOWN:   If (vpos < inst_vpos_max) then
                   begin
                     Repeat Inc(vpos) until (inst_hpos[vpos,1] <> 0);
                     temp := hpos; hpos := inst_hpos_max;
                     While (hpos > 1) and ((inst_hpos[vpos,hpos] = 0) or
                       (inst_hpos[vpos,hpos] > inst_hpos[vpos-1,temp])) do Dec(hpos);
                   end
                 else
                   begin
                     vpos := 1; temp := hpos; hpos := inst_hpos_max;
                     While (hpos > 1) and ((inst_hpos[vpos,hpos] = 0) or
                       (inst_hpos[vpos,hpos] > inst_hpos[vpos-1,temp])) do Dec(hpos)
                   end;

        kHOME:   begin vpos := 1; hpos := 1; end;

        kEND:    begin
                   vpos := inst_vpos_max; hpos := inst_hpos_max;
                   While (hpos > 1) and (inst_hpos[vpos,hpos] = 0) do Dec(hpos);
                 end;

        kTAB:    Case vpos of
                    1: begin vpos := 2; hpos := 1; end; // ATTACK RATE -> DECAY RATE

                    2: If (hpos = 1) then begin vpos := 4; hpos := 1; end // DECAY RATE -> SUSTAIN LEVEL
                       else begin vpos := 7; hpos := 1; end; // WAVEFORM TYPE -> OUTPUT LEVEL

                    3:  begin vpos := 7; hpos := 1; end; // WAVEFORM TYPE -> OUTPUT LEVEL

                    4: If (hpos = 1) then begin vpos := 5; hpos := 1; end // SUSTAIN LEVEL -> RELEASE RATE
                       else begin vpos := 7; hpos := 1; end; // WAVEFORM TYPE -> OUTPUT LEVEL

                    5: If (hpos = 1) then // RELEASE RATE -> WAVEFORM TYPE
                         begin
                           If carrier then vpos := 2+inst^.fm_data.WAVEFORM_carrier
                           else vpos := 2+inst^.fm_data.WAVEFORM_modulator;
                           If (vpos in [2,4,5,7]) then hpos := 2 else hpos := 1;
                         end
                       else begin vpos := 7; hpos := 1; end; // WAVEFORM TYPE -> OUTPUT LEVEL

                    6: begin vpos := 7; hpos := 1; end; // WAVEFORM TYPE -> OUTPUT LEVEL

                    7: If (hpos = 1) then // OUTPUT LEVEL -> KEY SCALiNG LEVEL
                         begin
                           If carrier then temp := inst^.fm_data.KSL_VOLUM_carrier
                           else temp := inst^.fm_data.KSL_VOLUM_modulator;
                           temp := temp SHR 6;
                           temp := temp SHR 1+temp AND 1 SHL 1;
                           vpos := 10+temp;
                           hpos := 1;
                         end
                       else begin vpos := 7; hpos := 1; end; // WAVEFORM TYPE -> OUTPUT LEVEL

                    8,
                    9: begin vpos := 7; hpos := 1; end; // WAVEFORM TYPE -> OUTPUT LEVEL

                   10,11,12,
                   13: If (hpos = 1) then // KEY SCALING LEVEL -> PANNING
                         Case inst^.panning of
                           1: begin vpos := 17; hpos := 1; end;
                           0: begin vpos := 17; hpos := 2; end;
                           2: begin vpos := 17; hpos := 3; end;
                         end
                       else If (vpos in [12,13]) and (hpos = 2) then // CONNECTION -> FEEDBACK
                              begin vpos := 12; hpos := 3; end
                            else begin vpos := 16; hpos := 2; end; // FEEDBACK -> F-TUNE

                   14,15,
                   16: If (hpos = 1) then // CONNECTION -> FEEDBACK
                         begin vpos := 12; hpos := 3; end
                       else begin vpos := 18; hpos := 1; end; // F-TUNE -> AM/VIB/EG/KSR

                   17: If (hpos in [1,2,3]) then
                         If NOT (inst^.perc_voice in [2..5]) then // PANNING -> CONNECTION
                           Case Pos(#11,_curr_connection_str) of
                             1: begin vpos := 12; hpos := 2; end;
                             2: begin vpos := 13; hpos := 2; end;
                             3: begin vpos := 14; hpos := 1; end;
                             4: begin vpos := 15; hpos := 1; end;
                             5: begin vpos := 16; hpos := 1; end;
                             6: begin vpos := 17; hpos := 3; end;
                           end
                         else begin vpos := 16; hpos := 2; end // PERC:SD/TT/TC/HH == PANNING -> F-TUNE
                       else begin vpos := 12; hpos := 3; end; // CONNECTION -> FEEDBACK

                   18,19,20,
                   21: If (vpos = 18) or (hpos = 1) then // AM/VIB/EG/KSR -> FREQ. DATA MULT.
                         begin
                           If carrier then temp := inst^.fm_data.AM_VIB_EG_carrier AND $0f
                           else temp := inst^.fm_data.AM_VIB_EG_modulator AND $0f;
                           hpos := 2+(temp MOD 6);
                           vpos := 19+(temp DIV 6);
                         end
                       else begin // *NEXT SLOT*
                              vpos := 1;
                              hpos := 1;
                              If NOT (inst^.perc_voice in [2..5]) then
                                begin
                                  carrier := NOT carrier;
                                  If carrier and (get_4op_to_test <> 0) then
                                    begin
                                      If (current_inst = LO(get_4op_to_test)) then current_inst := HI(get_4op_to_test)
                                      else current_inst := LO(get_4op_to_test);
                                      instrum_page := current_inst;
                                      STATUS_LINE_refresh;
                                      inst := Addr(songdata.instr_data[instrum_page]);
                                      _page_build;
                                      _page_refresh;
                                    end;
                                end;
                       end;
                 end;

        kShTAB:  Case vpos of
                    1: begin // *PREVIOUS SLOT*
                         If NOT (inst^.perc_voice in [2..5]) then
                           begin
                             carrier := NOT carrier;
                             If NOT carrier and (get_4op_to_test <> 0) then
                               begin
                                 If (current_inst = LO(get_4op_to_test)) then current_inst := HI(get_4op_to_test)
                                 else current_inst := LO(get_4op_to_test);
                                 instrum_page := current_inst;
                                 STATUS_LINE_refresh;
                                 inst := Addr(songdata.instr_data[instrum_page]);
                                 _page_build;
                                 _page_refresh;
                               end;
                           end;
                         If carrier then temp := inst^.fm_data.AM_VIB_EG_carrier AND $0f
                         else temp := inst^.fm_data.AM_VIB_EG_modulator AND $0f;
                         hpos := 2+(temp MOD 6);
                         vpos := 19+(temp DIV 6);
                       end;

                    2: If (hpos = 1) then begin vpos := 1; hpos := 1; end // DECAY RATE -> ATTACK RATE
                       else begin vpos := 5; hpos := 1; end; // WAVEFORM TYPE -> RELEASE RATE

                    3: begin vpos := 5; hpos := 1; end; // WAVEFORM TYPE -> RELEASE RATE

                    4: If (hpos = 1) then begin vpos := 2; hpos := 1; end
                       else begin vpos := 5; hpos := 1; end; // WAVEFORM TYPE -> RELEASE RATE

                    5: If (hpos = 1) then begin vpos := 4; hpos := 1; end
                       else begin vpos := 5; hpos := 1; end; // WAVEFORM TYPE -> RELEASE RATE

                    6: begin vpos := 5; hpos := 1; end; // WAVEFORM TYPE -> RELEASE RATE

                    7: If (hpos = 1) then // OUTPUT LEVEL -> WAVEFORM TYPE
                         begin
                           If carrier then vpos := 2+inst^.fm_data.WAVEFORM_carrier
                           else vpos := 2+inst^.fm_data.WAVEFORM_modulator;
                           If (vpos in [2,4,5,7]) then hpos := 2 else hpos := 1;
                         end
                       else begin vpos := 5; hpos := 1; end; // WAVEFORM TYPE -> RELEASE RATE
                    8,
                    9: begin vpos := 5; hpos := 1; end; // WAVEFORM TYPE -> RELEASE RATE

                   10,11,12,
                   13: If (hpos = 1) then // KEY SCALING RATE -> OUTPUT LEVEL
                         begin vpos := 7; hpos := 1; end
                       else If (hpos = 2) then // CONNECTION -> PANNING
                              Case inst^.panning of
                                1: begin vpos := 17; hpos := 1; end;
                                0: begin vpos := 17; hpos := 2; end;
                                2: begin vpos := 17; hpos := 3; end;
                              end
                            else Case Pos(#11,_curr_connection_str) of // FEEDBACK -> CONNECTION
                                   1: begin vpos := 12; hpos := 2; end;
                                   2: begin vpos := 13; hpos := 2; end;
                                   3: begin vpos := 14; hpos := 1; end;
                                   4: begin vpos := 15; hpos := 1; end;
                                   5: begin vpos := 16; hpos := 1; end;
                                   6: begin vpos := 17; hpos := 3; end;
                                 end;
                   14,15,
                   16: If (hpos = 1) then // CONNECTION -> PANNING
                         Case inst^.panning of
                           1: begin vpos := 17; hpos := 1; end;
                           0: begin vpos := 17; hpos := 2; end;
                           2: begin vpos := 17; hpos := 3; end;
                         end
                       else If NOT (inst^.perc_voice in [2..5]) then // F-TUNE -> FEEDBACK
                              begin vpos := 12; hpos := 3; end
                            else Case inst^.panning of // PERC:SD/TT/TC/HH == F-TUNE -> PANNING
                                   1: begin vpos := 17; hpos := 1; end;
                                   0: begin vpos := 17; hpos := 2; end;
                                   2: begin vpos := 17; hpos := 3; end;
                                 end;

                   17: If (hpos in [1,2,3]) then // PANNING -> KEY SCALING LEVEL
                         begin
                           If carrier then temp := inst^.fm_data.KSL_VOLUM_carrier
                           else temp := inst^.fm_data.KSL_VOLUM_modulator;
                           temp := temp SHR 6;
                           temp := temp SHR 1+temp AND 1 SHL 1;
                           vpos := 10+temp;
                           hpos := 1;
                         end
                       else Case inst^.panning of // CONNECTION -> PANNING
                              1: begin vpos := 17; hpos := 1; end;
                              0: begin vpos := 17; hpos := 2; end;
                              2: begin vpos := 17; hpos := 3; end;
                            end;

                   18,19,20,
                   21: If (vpos = 18) or (hpos = 1) then
                         begin vpos := 16; hpos := 2; end // AM/VIB/EG/KSR -> F-TUNE
                       else begin vpos := 18; hpos := 1; end; // FREQ. DATA MULT. -> AM/VIB/EG/KSR
                 end;

        kSPACE:  If ctrl_pressed then
                   _ADSR_preview_flag := NOT _ADSR_preview_flag
                 else
                   begin
                     Case vpos of
                       2: If (hpos = 2) then
                            If carrier then inst^.fm_data.WAVEFORM_carrier := 0
                            else inst^.fm_data.WAVEFORM_modulator := 0;

                       3: If carrier then inst^.fm_data.WAVEFORM_carrier := 1
                          else inst^.fm_data.WAVEFORM_modulator := 1;

                       4: If (hpos = 2) then
                            If carrier then inst^.fm_data.WAVEFORM_carrier := 2
                            else inst^.fm_data.WAVEFORM_modulator := 2;

                       5: If (hpos = 2) then
                            If carrier then inst^.fm_data.WAVEFORM_carrier := 3
                            else inst^.fm_data.WAVEFORM_modulator := 3;

                       6: If carrier then inst^.fm_data.WAVEFORM_carrier := 4
                          else inst^.fm_data.WAVEFORM_modulator := 4;

                       7: If (hpos = 2) then
                            If carrier then inst^.fm_data.WAVEFORM_carrier := 5
                            else inst^.fm_data.WAVEFORM_modulator := 5;

                       8: If carrier then inst^.fm_data.WAVEFORM_carrier := 6
                          else inst^.fm_data.WAVEFORM_modulator := 6;

                       9: If carrier then inst^.fm_data.WAVEFORM_carrier := 7
                          else inst^.fm_data.WAVEFORM_modulator := 7;

                      10..16,
                      17: If (vpos in [10..13]) and (hpos = 1) then
                            If carrier then
                              inst^.fm_data.KSL_VOLUM_carrier :=
                              inst^.fm_data.KSL_VOLUM_carrier AND $3f+KSL_volume[vpos-10]
                            else
                              inst^.fm_data.KSL_VOLUM_modulator :=
                              inst^.fm_data.KSL_VOLUM_modulator AND $3f+KSL_volume[vpos-10]
                          else If ((vpos in [12,13]) and (hpos = 2)) or
                                  ((vpos in [14,15,16]) and (hpos = 1)) or
                                  ((vpos = 17) and (hpos = 4)) then
                                 Case vpos  of
                                   12,13: If NOT (get_4op_to_test <> 0) then
                                            inst^.fm_data.FEEDBACK_FM := vpos-12+inst^.fm_data.FEEDBACK_FM AND $0e;
                                   14: If (get_4op_to_test <> 0) then
                                         begin
                                           pBYTE(@Addr(songdata.instr_data[HI(get_4op_to_test)])^)[_instr_data_ofs[11]] := 0+
                                           pBYTE(@Addr(songdata.instr_data[HI(get_4op_to_test)])^)[_instr_data_ofs[11]] AND $0e;
                                           pBYTE(@Addr(songdata.instr_data[LO(get_4op_to_test)])^)[_instr_data_ofs[11]] := 0+
                                           pBYTE(@Addr(songdata.instr_data[LO(get_4op_to_test)])^)[_instr_data_ofs[11]] AND $0e;
                                         end;
                                   15: If (get_4op_to_test <> 0) then
                                         begin
                                           pBYTE(@Addr(songdata.instr_data[HI(get_4op_to_test)])^)[_instr_data_ofs[11]] := 0+
                                           pBYTE(@Addr(songdata.instr_data[HI(get_4op_to_test)])^)[_instr_data_ofs[11]] AND $0e;
                                           pBYTE(@Addr(songdata.instr_data[LO(get_4op_to_test)])^)[_instr_data_ofs[11]] := 1+
                                           pBYTE(@Addr(songdata.instr_data[LO(get_4op_to_test)])^)[_instr_data_ofs[11]] AND $0e;
                                         end;
                                   16: If (get_4op_to_test <> 0) then
                                         begin
                                           pBYTE(@Addr(songdata.instr_data[HI(get_4op_to_test)])^)[_instr_data_ofs[11]] := 1+
                                           pBYTE(@Addr(songdata.instr_data[HI(get_4op_to_test)])^)[_instr_data_ofs[11]] AND $0e;
                                           pBYTE(@Addr(songdata.instr_data[LO(get_4op_to_test)])^)[_instr_data_ofs[11]] := 0+
                                           pBYTE(@Addr(songdata.instr_data[LO(get_4op_to_test)])^)[_instr_data_ofs[11]] AND $0e;
                                         end;
                                   17: If (get_4op_to_test <> 0) then
                                         begin
                                           pBYTE(@Addr(songdata.instr_data[HI(get_4op_to_test)])^)[_instr_data_ofs[11]] := 1+
                                           pBYTE(@Addr(songdata.instr_data[HI(get_4op_to_test)])^)[_instr_data_ofs[11]] AND $0e;
                                           pBYTE(@Addr(songdata.instr_data[LO(get_4op_to_test)])^)[_instr_data_ofs[11]] := 1+
                                           pBYTE(@Addr(songdata.instr_data[LO(get_4op_to_test)])^)[_instr_data_ofs[11]] AND $0e;
                                         end;
                                 end
                               else If (vpos = 17) and (hpos in [1..3]) then
                                      inst^.panning := panning_pos[hpos-1];
                      18,19,20,
                      21: If (vpos = 18) or (hpos = 1) then
                            begin
                              If carrier then
                                inst^.fm_data.AM_VIB_EG_carrier :=
                                inst^.fm_data.AM_VIB_EG_carrier XOR AM_Vibrato_EG[vpos-18]
                              else
                                inst^.fm_data.AM_VIB_EG_modulator :=
                                inst^.fm_data.AM_VIB_EG_modulator XOR AM_Vibrato_EG[vpos-18];
                            end
                          else
                            begin
                              If carrier then
                                inst^.fm_data.AM_VIB_EG_carrier :=
                                inst^.fm_data.AM_VIB_EG_carrier AND $0f0+
                                (vpos-19)*6+hpos-2
                              else
                                inst^.fm_data.AM_VIB_EG_modulator :=
                                inst^.fm_data.AM_VIB_EG_modulator AND $0f0+
                                (vpos-19)*6+hpos-2;
                            end;
                     end;
                     reset_instrument_preview;
                   end;
        kPgUP,
        kCHplus,
        kNPplus: begin
                   If carrier then
                     begin
                       If (vpos = 1) and
                          (inst^.fm_data.ATTCK_DEC_carrier SHR 4 < $0f) then
                           inst^.fm_data.ATTCK_DEC_carrier :=
                           inst^.fm_data.ATTCK_DEC_carrier AND $0f+
                          (inst^.fm_data.ATTCK_DEC_carrier SHR 4 +1) SHL 4;

                       If (vpos = 2) and (hpos = 1) and
                          (inst^.fm_data.ATTCK_DEC_carrier AND $0f < $0f) then
                           Inc(inst^.fm_data.ATTCK_DEC_carrier);

                       If (vpos = 4) and (hpos = 1) and
                          (inst^.fm_data.SUSTN_REL_carrier SHR 4 < $0f) then
                           inst^.fm_data.SUSTN_REL_carrier :=
                           inst^.fm_data.SUSTN_REL_carrier AND $0f+
                          (inst^.fm_data.SUSTN_REL_carrier SHR 4 +1) SHL 4;

                       If (vpos = 5) and (hpos = 1) and
                          (inst^.fm_data.SUSTN_REL_carrier AND $0f < $0f) then
                           Inc(inst^.fm_data.SUSTN_REL_carrier);

                       If (vpos = 7) and (hpos = 1) and
                          (inst^.fm_data.KSL_VOLUM_carrier AND $3f > 0) then
                           Dec(inst^.fm_data.KSL_VOLUM_carrier);
                     end
                   else
                     begin
                       If (vpos = 1) and
                          (inst^.fm_data.ATTCK_DEC_modulator SHR 4 < $0f) then
                           inst^.fm_data.ATTCK_DEC_modulator :=
                           inst^.fm_data.ATTCK_DEC_modulator AND $0f+
                          (inst^.fm_data.ATTCK_DEC_modulator SHR 4 +1) SHL 4;

                       If (vpos = 2) and (hpos = 1) and
                          (inst^.fm_data.ATTCK_DEC_modulator AND $0f < $0f) then
                           Inc(inst^.fm_data.ATTCK_DEC_modulator);

                       If (vpos = 4) and (hpos = 1) and
                          (inst^.fm_data.SUSTN_REL_modulator SHR 4 < $0f) then
                           inst^.fm_data.SUSTN_REL_modulator :=
                           inst^.fm_data.SUSTN_REL_modulator AND $0f+
                          (inst^.fm_data.SUSTN_REL_modulator SHR 4 +1) SHL 4;

                       If (vpos = 5) and (hpos = 1) and
                          (inst^.fm_data.SUSTN_REL_modulator AND $0f < $0f) then
                           Inc(inst^.fm_data.SUSTN_REL_modulator);

                       If (vpos = 7) and (hpos = 1) and
                          (inst^.fm_data.KSL_VOLUM_modulator AND $3f > 0) then
                           Dec(inst^.fm_data.KSL_VOLUM_modulator);
                     end;

                   If (vpos = 12) and (hpos = 3) then _inc_feedback_val;
                   If (vpos = 16) and (hpos = 2) then _inc_finetune_val;
                   reset_instrument_preview;
                 end;

        kPgDOWN,
        kCHmins,
        kNPmins: begin
                   If carrier then
                     begin
                       If (vpos = 1) and
                          (inst^.fm_data.ATTCK_DEC_carrier SHR 4 > 0) then
                           inst^.fm_data.ATTCK_DEC_carrier :=
                           inst^.fm_data.ATTCK_DEC_carrier AND $0f+
                          (inst^.fm_data.ATTCK_DEC_carrier SHR 4 -1) SHL 4;

                       If (vpos = 2) and (hpos = 1) and
                          (inst^.fm_data.ATTCK_DEC_carrier AND $0f > 0) then
                           Dec(inst^.fm_data.ATTCK_DEC_carrier);

                       If (vpos = 4) and (hpos = 1) and
                          (inst^.fm_data.SUSTN_REL_carrier SHR 4 > 0) then
                           inst^.fm_data.SUSTN_REL_carrier :=
                           inst^.fm_data.SUSTN_REL_carrier AND $0f+
                          (inst^.fm_data.SUSTN_REL_carrier SHR 4 -1) SHL 4;

                       If (vpos = 5) and (hpos = 1) and
                          (inst^.fm_data.SUSTN_REL_carrier AND $0f > 0) then
                           Dec(inst^.fm_data.SUSTN_REL_carrier);

                       If (vpos = 7) and (hpos = 1) and
                          (inst^.fm_data.KSL_VOLUM_carrier AND $3f < $3f) then
                           Inc(inst^.fm_data.KSL_VOLUM_carrier);
                     end
                   else
                     begin
                       If (vpos = 1) and
                          (inst^.fm_data.ATTCK_DEC_modulator SHR 4 > 0) then
                           inst^.fm_data.ATTCK_DEC_modulator :=
                           inst^.fm_data.ATTCK_DEC_modulator AND $0f+
                          (inst^.fm_data.ATTCK_DEC_modulator SHR 4 -1) SHL 4;

                       If (vpos = 2) and (hpos = 1) and
                          (inst^.fm_data.ATTCK_DEC_modulator AND $0f > 0) then
                           Dec(inst^.fm_data.ATTCK_DEC_modulator);

                       If (vpos = 4) and (hpos = 1) and
                          (inst^.fm_data.SUSTN_REL_modulator SHR 4 > 0) then
                           inst^.fm_data.SUSTN_REL_modulator :=
                           inst^.fm_data.SUSTN_REL_modulator AND $0f+
                          (inst^.fm_data.SUSTN_REL_modulator SHR 4 -1) SHL 4;

                       If (vpos = 5) and (hpos = 1) and
                          (inst^.fm_data.SUSTN_REL_modulator AND $0f > 0) then
                           Dec(inst^.fm_data.SUSTN_REL_modulator);

                       If (vpos = 7) and (hpos = 1) and
                          (inst^.fm_data.KSL_VOLUM_modulator AND $3f < $3f) then
                           Inc(inst^.fm_data.KSL_VOLUM_modulator);
                     end;

                   If (vpos = 12) and (hpos = 3) then _dec_feedback_val;
                   If (vpos = 16) and (hpos = 2) then _dec_finetune_val;
                   reset_instrument_preview;
                 end;

      kCtLbr:  If shift_pressed then
                 begin
                   If (songdata.macro_speedup > 1) then
                     Dec(songdata.macro_speedup);
                   macro_speedup := songdata.macro_speedup;
                   keyboard_reset_buffer;
                 end
               else If (current_inst > 1) then
                      begin
                        Dec(current_inst);
                        reset_marked_instruments;
                        instrum_page := current_inst;
                        STATUS_LINE_refresh;
                        inst := Addr(songdata.instr_data[instrum_page]);
                        If NOT (inst^.perc_voice in [2..5]) then carrier := TRUE
                        else carrier := FALSE;
                        FillChar(_operator_enabled,SizeOf(_operator_enabled),TRUE);
                        _page_build;
                        _page_refresh;
                        _sync_radio_buttons;
                      end;

      kCtRbr:  If shift_pressed then
                 begin
                   Inc(songdata.macro_speedup);
                   If (calc_max_speedup(songdata.tempo) < songdata.macro_speedup) then
                     songdata.macro_speedup := calc_max_speedup(songdata.tempo);
                   macro_speedup := songdata.macro_speedup;
                   keyboard_reset_buffer;
                 end
               else If (current_inst < 255) then
                      begin
                        Inc(current_inst);
                        reset_marked_instruments;
                        instrum_page := current_inst;
                        STATUS_LINE_refresh;
                        inst := Addr(songdata.instr_data[instrum_page]);
                        If NOT (inst^.perc_voice in [2..5]) then carrier := TRUE
                        else carrier := FALSE;
                        FillChar(_operator_enabled,SizeOf(_operator_enabled),TRUE);
                        _page_build;
                        _page_refresh;
                        _sync_radio_buttons;
                      end;

        kF1:     begin
                   Dec(xstart);
                   HideCursor;
                   move_to_screen_data := ptr_screen_backup;
                   move_to_screen_area[1] := xstart;
                   move_to_screen_area[2] := ystart;
                   move_to_screen_area[3] := xstart+71+2;
                   move_to_screen_area[4] := ystart+24+INSEDIT_yshift+1;
                   move2screen;
                   HELP('instrument_editor');
                   GOTO _jmp1;
                 end;
        kF2,
        kCtrlS:  begin
                   Dec(xstart);
                   HideCursor;
                   move_to_screen_data := ptr_screen_backup;
                   move_to_screen_area[1] := xstart;
                   move_to_screen_area[2] := ystart;
                   move_to_screen_area[3] := xstart+71+2;
                   move_to_screen_area[4] := ystart+24+INSEDIT_yshift+1;
                   move2screen;

                   quick_cmd := FALSE;
                   FILE_save('a2i');
                   GOTO _jmp1;
                 end;

        kShF2:   begin
                   Dec(xstart);
                   HideCursor;
                   move_to_screen_data := ptr_screen_backup;
                   move_to_screen_area[1] := xstart;
                   move_to_screen_area[2] := ystart;
                   move_to_screen_area[3] := xstart+71+2;
                   move_to_screen_area[4] := ystart+24+INSEDIT_yshift+1;
                   move2screen;

                   quick_cmd := FALSE;
                   FILE_save('a2f');
                   GOTO _jmp1;
                 end;
        kF3,
        kCtrlL:  begin
                   Dec(xstart);
                   HideCursor;
                   move_to_screen_data := ptr_screen_backup;
                   move_to_screen_area[1] := xstart;
                   move_to_screen_area[2] := ystart;
                   move_to_screen_area[3] := xstart+71+2;
                   move_to_screen_area[4] := ystart+24+INSEDIT_yshift+1;
                   move2screen;

                   FillChar(_operator_enabled,SizeOf(_operator_enabled),TRUE);
                   quick_cmd := FALSE;
                   FILE_open('*.a2i$*.a2f$*.a2b$*.a2w$'+
                             '*.bnk$*.cif$*.fib$*.fin$*.ibk$*.ins$*.sbi$*.sgi$',FALSE);// load bank is not possible
                   reset_instrument_preview;
                   GOTO _jmp1;
                 end;

        kF7:     For temp := 1 to 20 do reset_chan_data(temp);

        kCtrlO:  begin
                   Dec(xstart);
                   HideCursor;
                   move_to_screen_data := ptr_screen_backup;
                   move_to_screen_area[1] := xstart;
                   move_to_screen_area[2] := ystart;
                   move_to_screen_area[3] := xstart+71+2;
                   move_to_screen_area[4] := ystart+24+INSEDIT_yshift+1;
                   move2screen;
                   OCTAVE_CONTROL;
                   GOTO _jmp1;
                 end;

        kCtrlQ:  begin
                   Dec(xstart);
                   HideCursor;
                   move_to_screen_data := ptr_screen_backup;
                   move_to_screen_area[1] := xstart;
                   move_to_screen_area[2] := ystart;
                   move_to_screen_area[3] := xstart+71+2;
                   move_to_screen_area[4] := ystart+24+INSEDIT_yshift+1;
                   move2screen;
                   MACRO_EDITOR(current_inst,FALSE);
                   GOTO _jmp1;
                 end;

        kCtrlE:  begin
                   Dec(xstart);
                   HideCursor;
                   move_to_screen_data := ptr_screen_backup;
                   move_to_screen_area[1] := xstart;
                   move_to_screen_area[2] := ystart;
                   move_to_screen_area[3] := xstart+71+2;
                   move_to_screen_area[4] := ystart+24+INSEDIT_yshift+1;
                   move2screen;
                   MACRO_EDITOR(current_inst,TRUE);
                   GOTO _jmp1;
                 end;

        kENTER:  If NOT (inst^.perc_voice in [2..5]) then
                   If NOT shift_pressed then
                     begin
                       If remember_ins_pos then
                         If carrier then
                           begin
                             add_bank_position('?instrument_editor?'+byte2hex(instrum_page)+'?carrier?hpos',-1,hpos);
                             add_bank_position('?instrument_editor?'+byte2hex(instrum_page)+'?carrier?vpos',-1,vpos);
                           end
                         else begin
                                add_bank_position('?instrument_editor?'+byte2hex(instrum_page)+'?modulator?hpos',-1,hpos);
                                add_bank_position('?instrument_editor?'+byte2hex(instrum_page)+'?modulator?vpos',-1,vpos);
                              end;
                       carrier := NOT carrier;
                       If carrier and
                          (get_4op_to_test <> 0) then
                         begin
                           If (current_inst = LO(get_4op_to_test)) then current_inst := HI(get_4op_to_test)
                           else current_inst := LO(get_4op_to_test);
                           instrum_page := current_inst;
                           STATUS_LINE_refresh;
                           inst := Addr(songdata.instr_data[instrum_page]);
                           _page_build;
                           _page_refresh;
                         end;
                       _sync_radio_buttons;
                     end
                   else
                     With songdata.instr_data[instrum_page].fm_data do
                       If carrier then
                         begin
                           ATTCK_DEC_carrier := ATTCK_DEC_modulator;
                           AM_VIB_EG_carrier := AM_VIB_EG_modulator;
                           KSL_VOLUM_carrier := KSL_VOLUM_modulator;
                           SUSTN_REL_carrier := SUSTN_REL_modulator;
                           WAVEFORM_carrier  := WAVEFORM_modulator;
                         end
                       else
                         begin
                           AM_VIB_EG_modulator := AM_VIB_EG_carrier;
                           KSL_VOLUM_modulator := KSL_VOLUM_carrier;
                           ATTCK_DEC_modulator := ATTCK_DEC_carrier;
                           SUSTN_REL_modulator := SUSTN_REL_carrier;
                           WAVEFORM_modulator  := WAVEFORM_carrier;
                         end;
        else nope := TRUE;
      end;

    If shift_pressed and (UpCase(CHR(LO(fkey))) in ['M','O','B','S','T','C','H']) then
      begin
        If (UpCase(CHR(LO(fkey))) = 'O') then
          begin
            songdata.instr_data[instrum_page].perc_voice := 0;
            If (instrum_page <> 255) then songdata.instr_data[SUCC(instrum_page)].perc_voice := 0
            else songdata.instr_data[PRED(instrum_page)].perc_voice := 0;
            If (instrum_page <> 255) then
              If check_4op_flag(instrum_page) then
                reset_4op_flag(instrum_page)
              else set_4op_flag(instrum_page)
            else If check_4op_flag(PRED(instrum_page)) then
                   reset_4op_flag(PRED(instrum_page))
                 else set_4op_flag(PRED(instrum_page));
            update_4op_flag_marks;
          end
        else If NOT check_4op_flag(instrum_page) then
               Case UpCase(CHR(LO(fkey))) of
                 'M': inst^.perc_voice := 0;
                 'B': inst^.perc_voice := 1;
                 'S': inst^.perc_voice := 2;
                 'T': inst^.perc_voice := 3;
                 'C': inst^.perc_voice := 4;
                 'H': inst^.perc_voice := 5;
               end;
        update_4op_flag_marks;
      end;
_end:
{$IFDEF GO32V2}
    keyboard_reset_buffer_alt;
{$ELSE}
    draw_screen;
{$ENDIF}
  until (nope and (fkey = kESC)) or _force_program_quit;

  FillChar(_operator_enabled,SizeOf(_operator_enabled),TRUE);
  If remember_ins_pos then
    begin
      add_bank_position('?instrument_editor?'+byte2hex(instrum_page)+'?carrier',-1,_carrier_flag[carrier]);
      If carrier then
        begin
          add_bank_position('?instrument_editor?'+byte2hex(instrum_page)+'?carrier?hpos',-1,hpos);
          add_bank_position('?instrument_editor?'+byte2hex(instrum_page)+'?carrier?vpos',-1,vpos);
        end
      else begin
             add_bank_position('?instrument_editor?'+byte2hex(instrum_page)+'?modulator?hpos',-1,hpos);
             add_bank_position('?instrument_editor?'+byte2hex(instrum_page)+'?modulator?vpos',-1,vpos);
           end;
    end;

  Dec(xstart);
  HideCursor;
  move_to_screen_data := ptr_screen_backup;
  move_to_screen_area[1] := xstart;
  move_to_screen_area[2] := ystart;
  move_to_screen_area[3] := xstart+71+2;
  move_to_screen_area[4] := ystart+24+INSEDIT_yshift+1;
  move2screen;

  _dbg_leave; //EXIT //INSTRUMENT_CONTROL_edit
end;
