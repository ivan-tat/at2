// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure _page_build;

var
  temp,nm_slots: Byte;
  temp_str: String;
  _pan_4op: String;
  _feedb_4op: String;
  _ftune_4op: String;

begin
  _dbg_enter ({$I %FILE%}, '(INSTRUMENT_CONTROL_edit)._page_build');

  ShowStr(ptr_temp_screen,xstart+45,ystart,
          byte2hex(instrum_page),
          instrument_bckg+instrument_title);

  If NOT is_default_screen_mode then
    ShowStr(ptr_temp_screen,xstart-1,ystart+24,
            #204+ExpStrL('',70,#205)+#185,
            instrument_bckg+instrument_border);

  If NOT (inst^.perc_voice in [2..5]) and (get_4op_to_test <> 0) then
    begin
      If (LO(get_4op_to_test) = HI(get_4op_to_test)) then
        _4op_conn_str := connection_str[pBYTE(@Addr(songdata.instr_data[HI(get_4op_to_test)])^)[_instr_data_ofs[11]] AND 1]+'/'+
                         connection_str[pBYTE(@Addr(songdata.instr_data[HI(get_4op_to_test)])^)[_instr_data_ofs[11]] AND 1]
      else If (current_inst = HI(get_4op_to_test)) then
             _4op_conn_str := '~'+connection_str[pBYTE(@Addr(songdata.instr_data[HI(get_4op_to_test)])^)[_instr_data_ofs[11]] AND 1]+'~/'+
                              connection_str[pBYTE(@Addr(songdata.instr_data[LO(get_4op_to_test)])^)[_instr_data_ofs[11]] AND 1]
           else
             _4op_conn_str := connection_str[pBYTE(@Addr(songdata.instr_data[HI(get_4op_to_test)])^)[_instr_data_ofs[11]] AND 1]+'/~'+
                               connection_str[pBYTE(@Addr(songdata.instr_data[LO(get_4op_to_test)])^)[_instr_data_ofs[11]] AND 1]+'~';
      If (current_inst = LO(get_4op_to_test)) then
        If NOT carrier then
          If (HI(get_4op_to_test) <> LO(get_4op_to_test)) then
            begin
              _4op_slot_str := '~1~`'#27'`2`'#27'`3`'#27'`4';
              _4op_slot_chr := ['1'];
            end
          else begin
                 _4op_slot_str := '~1~`'#27'`2`'#27'`~3~`'#27'`4';
                 _4op_slot_chr := ['1','3']
               end
        else If (HI(get_4op_to_test) <> LO(get_4op_to_test)) then
               begin
                 _4op_slot_str := '1`'#27'`~2~`'#27'`3`'#27'`4';
                 _4op_slot_chr := ['2'];
               end
             else begin
                    _4op_slot_str := '1`'#27'`~2~`'#27'`3`'#27'`~4~';
                    _4op_slot_chr := ['2','4'];
                  end
      else If NOT carrier then
             If (HI(get_4op_to_test) <> LO(get_4op_to_test)) then
               begin
                 _4op_slot_str := '1`'#27'`2`'#27'`~3~`'#27'`4';
                 _4op_slot_chr := ['3'];
               end
             else begin
                    _4op_slot_str := '~1~`'#27'`2`'#27'`~3~`'#27'`4';
                    _4op_slot_chr := ['1','3'];
                  end
           else If (HI(get_4op_to_test) <> LO(get_4op_to_test)) then
                  begin
                    _4op_slot_str := '1`'#27'`2`'#27'`3`'#27'`~4~';
                    _4op_slot_chr := ['4'];
                  end
                else begin
                       _4op_slot_str := '1`'#27'`~2~`'#27'`3`'#27'`~4~';
                       _4op_slot_chr := ['2','4'];
                     end;
    end;

  If (inst^.perc_voice in [2..5]) then
    begin
      Move(inst_text_ext_perc,inst_text[9],SizeOf(inst_text_ext_perc));
      Move(inst_hpos_ext_perc,inst_hpos[12],SizeOf(inst_hpos_ext_perc));
    end
  else If NOT (get_4op_to_test <> 0) then
         begin
           Move(inst_text_ext_2op,inst_text[9],SizeOf(inst_text_ext_2op));
           Move(inst_hpos_ext_2op,inst_hpos[12],SizeOf(inst_hpos_ext_2op));
         end
       else begin
              Move(inst_text_ext_4op,inst_text[9],SizeOf(inst_text_ext_4op));
              Move(inst_hpos_ext_4op,inst_hpos[12],SizeOf(inst_hpos_ext_4op));
            end;

  If NOT (inst^.perc_voice in [2..5]) and
     ((NOT (get_4op_to_test <> 0) and carrier) or
      ((get_4op_to_test <> 0) and (('2' in _4op_slot_chr) or ('4' in _4op_slot_chr)))) then
    For temp := 1 to 24 do
      ShowC4Str(ptr_temp_screen,xstart+1,ystart+temp,inst_text[temp],
                instrument_bckg+instrument_text,
                instrument_bckg+instrument_car,
                instrument_bckg+instrument_glob,
                instrument_bckg+instrument_hid)
  else
    For temp := 1 to 24 do
      ShowC4Str(ptr_temp_screen,xstart+1,ystart+temp,inst_text[temp],
                instrument_bckg+instrument_text,
                instrument_bckg+instrument_mod,
                instrument_bckg+instrument_glob,
                instrument_bckg+instrument_hid);

  If (inst^.perc_voice in [2..5]) then _status_shift_pos := 5
  else If NOT (get_4op_to_test <> 0) then _status_shift_pos := 9
       else _status_shift_pos := 19;

  If NOT (inst^.perc_voice in [2..5]) and (get_4op_to_test <> 0) then
    begin
      If (LO(get_4op_to_test) = HI(get_4op_to_test)) then
        ShowC3Str(ptr_temp_screen,xstart,ystart+24,
                  ' `[`'#244+byte2hex(HI(get_4op_to_test))+
                  ','#245+byte2hex(HI(get_4op_to_test))+' '+_4op_conn_str+'`]` ',
                  instrument_bckg+instrument_context,
                  instrument_context SHL 4,
                  instrument_bckg+instrument_border)
      else If (current_inst = HI(get_4op_to_test)) then
             ShowC3Str(ptr_temp_screen,xstart,ystart+24,
                       ' `[`~'#244+byte2hex(HI(get_4op_to_test))+
                       '~,'#245+byte2hex(LO(get_4op_to_test))+' '+_4op_conn_str+'`]` ',
                       instrument_bckg+instrument_context,
                       instrument_context SHL 4,
                       instrument_bckg+instrument_border)
           else ShowC3Str(ptr_temp_screen,xstart,ystart+24,
                          ' `[`'#244+byte2hex(HI(get_4op_to_test))+
                          ',~'#245+byte2hex(LO(get_4op_to_test))+'~ '+_4op_conn_str+'`]` ',
                          instrument_bckg+instrument_context,
                          instrument_context SHL 4,
                          instrument_bckg+instrument_border);
      _show_conn_scheme(xstart+32,ystart+11,_4op_conn_to_idx(_4op_conn_str),_4op_slot_chr);
      ShowC3Str(ptr_temp_screen,xstart+17,ystart+24,'`[`'#4#3':'+_4op_slot_str+'`]` ',
                instrument_bckg+instrument_context,
                instrument_context SHL 4,
                instrument_bckg+instrument_border);
    end
  else
    begin
      If NOT (inst^.perc_voice in [2..5]) then
        _show_conn_scheme(xstart+32,ystart+11,inst^.fm_data.FEEDBACK_FM AND 1,[CHR(BYTE(carrier)+ORD('1'))])
      else _show_conn_scheme(xstart+32,ystart+11,6,['1']);
      Case inst^.perc_voice of
        0: ShowCStr(ptr_temp_screen,xstart,ystart+24,' [~MELODiC~]',
                    instrument_bckg+instrument_border,
                    instrument_bckg+instrument_context);
        1: ShowCStr(ptr_temp_screen,xstart,ystart+24,' [~PERC:BD~]',
                    instrument_bckg+instrument_border,
                    instrument_bckg+instrument_context);
        2: ShowCStr(ptr_temp_screen,xstart,ystart+24,' [~PERC:SD~]',
                    instrument_bckg+instrument_border,
                    instrument_bckg+instrument_context);
        3: ShowCStr(ptr_temp_screen,xstart,ystart+24,' [~PERC:TT~]',
                    instrument_bckg+instrument_border,
                    instrument_bckg+instrument_context);
        4: ShowCStr(ptr_temp_screen,xstart,ystart+24,' [~PERC:TC~]',
                    instrument_bckg+instrument_border,
                    instrument_bckg+instrument_context);
        5: ShowCStr(ptr_temp_screen,xstart,ystart+24,' [~PERC:HH~]',
                    instrument_bckg+instrument_border,
                    instrument_bckg+instrument_context);
      end;
      If (inst^.perc_voice in [2..5]) then
        ShowCStr(ptr_temp_screen,xstart+10,ystart+24,
                 ' [~'#1#3'~] ',
                 instrument_bckg+instrument_border,
                 instrument_bckg+instrument_context)
      else
        If NOT carrier then
          ShowC3Str(ptr_temp_screen,xstart+10,ystart+24,' `[`'#2#3':~1~`'#27'`2`]`',
                    instrument_bckg+instrument_context,
                    instrument_context SHL 4,
                    instrument_bckg+instrument_border)
        else
          ShowC3Str(ptr_temp_screen,xstart+10,ystart+24,' `[`'#2#3':1`'#27'`~2~`]`',
                    instrument_bckg+instrument_context,
                    instrument_context SHL 4,
                    instrument_bckg+instrument_border);
    end;

  If (songdata.instr_macros[instrum_page].length <> 0) then temp_str := ' [~MACRO:FM'
  else temp_str := ' ';

  With songdata.macro_table[
       songdata.instr_macros[instrum_page].arpeggio_table].arpeggio do
    If (songdata.instr_macros[instrum_page].arpeggio_table <> 0) then
      If (temp_str <> ' ') then temp_str := temp_str+'+ARP'
      else temp_str := temp_str+'[~MACRO:ARP';

  With songdata.macro_table[
       songdata.instr_macros[instrum_page].vibrato_table].vibrato do
    If (songdata.instr_macros[instrum_page].vibrato_table <> 0) then
      If (temp_str <> ' ') then temp_str := temp_str+'+ViB'
      else temp_str := temp_str+'[~MACRO:ViB';

  If (temp_str <> ' ') then temp_str := temp_str+'~] ';

  ShowCStr(ptr_temp_screen,
           xstart+10+_status_shift_pos,ystart+24,
           ExpStrR(temp_str,39,#205),
           instrument_bckg+instrument_border,
           instrument_bckg+instrument_context);

   If (inst^.perc_voice in [2..5]) then
     begin
       temp_str := '```` ';
       nm_slots := 1;
     end
   else If NOT (get_4op_to_test <> 0) then
          begin
            temp_str := ' `[`12`]` ';
            nm_slots := 2;
          end
        else begin
               temp_str := ' `[`1234`]` ';
               nm_slots := 4;
             end;

  temp_str := temp_str+'[~SPEED:'+Num2str(songdata.tempo*songdata.macro_speedup,10)+#174+'~] ';
  ShowC3Str(ptr_temp_screen,xstart+48,ystart+24,
            ExpStrL(temp_str,28,#205),
            instrument_bckg+instrument_border,
            instrument_bckg+instrument_context,
            instrument_bckg+instrument_con_dis);

  If (nm_slots > 1) then
    For temp := 1 to nm_slots do
      If (NOT _operator_enabled[temp]) then
        ShowStr(ptr_temp_screen,xstart+72-C3StrLen(temp_str)-1+temp,ystart+24,
                #250,
                instrument_bckg+instrument_border);

  If (inst_hpos[vpos,hpos] = 0) then
    begin
      vpos := 1;
      hpos := 1;
    end;

  If NOT (get_4op_to_test <> 0) then
    begin
      _pan_4op := '     ';
      _feedb_4op :=  '    ';
      _ftune_4op :=  '    ';
    end
  else begin
         If (current_inst = HI(get_4op_to_test)) then
           _pan_4op := '('#244#2#3')'
         else _pan_4op := '('#245#2#3')';
         _feedb_4op := '['#4#3']';
         _ftune_4op := '['#4#3']'
       end;

  If NOT (inst^.perc_voice in [2..5]) then
    begin
      _col_mod := instrument_mod;
      _col_hi_mod := instrument_hi_mod;
    end
  else begin
         _col_mod := instrument_glob;
         _col_hi_mod := instrument_hi_glob;
       end;

  If NOT ((vpos = 1) and (hpos = 1)) then
    If carrier then
      ShowCStr(ptr_temp_screen,xstart+1,ystart+1,'~A~TTACK RATE',
               instrument_bckg+instrument_car,
               instrument_bckg+instrument_hi_car)
    else
      ShowCStr(ptr_temp_screen,xstart+1,ystart+1,'~A~TTACK RATE',
               instrument_bckg+_col_mod,
               instrument_bckg+_col_hi_mod);

  If NOT ((vpos = 2) and (hpos = 1)) then
    If carrier then
      ShowCStr(ptr_temp_screen,xstart+1,ystart+2,'~D~ECAY RATE',
               instrument_bckg+instrument_car,
               instrument_bckg+instrument_hi_car)
    else
      ShowCStr(ptr_temp_screen,xstart+1,ystart+2,'~D~ECAY RATE',
               instrument_bckg+_col_mod,
               instrument_bckg+_col_hi_mod);

  If NOT ((vpos in [3,6,8,9]) and (hpos = 1)) then
    If carrier then
      ShowCStr(ptr_temp_screen,xstart+51,ystart+1,'~W~AVEFORM TYPE',
               instrument_bckg+instrument_car,
               instrument_bckg+instrument_hi_car)
    else
      ShowCStr(ptr_temp_screen,xstart+51,ystart+1,'~W~AVEFORM TYPE',
               instrument_bckg+_col_mod,
               instrument_bckg+_col_hi_mod);

  If NOT ((vpos = 4) and (hpos = 1)) then
    If carrier then
      ShowCStr(ptr_temp_screen,xstart+1,ystart+4,'~S~USTAiN LEVEL',
               instrument_bckg+instrument_car,
               instrument_bckg+instrument_hi_car)
    else
      ShowCStr(ptr_temp_screen,xstart+1,ystart+4,'~S~USTAiN LEVEL',
               instrument_bckg+_col_mod,
               instrument_bckg+_col_hi_mod);

  If NOT ((vpos = 5) and (hpos = 1)) then
    If carrier then
      ShowCStr(ptr_temp_screen,xstart+1,ystart+5,'~R~ELEASE RATE',
               instrument_bckg+instrument_car,
               instrument_bckg+instrument_hi_car)
    else
      ShowCStr(ptr_temp_screen,xstart+1,ystart+5,'~R~ELEASE RATE',
               instrument_bckg+_col_mod,
               instrument_bckg+_col_hi_mod);

  If NOT ((vpos = 7) and (hpos = 1)) then
    If carrier then
      ShowCStr(ptr_temp_screen,xstart+1,ystart+7,'~O~UTPUT LEVEL',
               instrument_bckg+instrument_car,
               instrument_bckg+instrument_hi_car)
    else
      ShowCStr(ptr_temp_screen,xstart+1,ystart+7,'~O~UTPUT LEVEL',
               instrument_bckg+_col_mod,
               instrument_bckg+_col_hi_mod);

  If NOT ((vpos in [10..13]) and (hpos = 1)) then
    If carrier then
      ShowCStr(ptr_temp_screen,xstart+1,ystart+9,'~K~EY SCALiNG LEVEL',
               instrument_bckg+instrument_car,
               instrument_bckg+instrument_hi_car)
    else
      ShowCStr(ptr_temp_screen,xstart+1,ystart+9,'~K~EY SCALiNG LEVEL',
               instrument_bckg+_col_mod,
               instrument_bckg+_col_hi_mod);

  If NOT (inst^.perc_voice in [2..5]) then
    begin
      If NOT (((vpos in [12,13]) and (hpos = 2)) or
              ((vpos in [14,15]) and (hpos = 1)) or
              ((vpos = 17) and (hpos = 4))) then
        ShowCStr(ptr_temp_screen,xstart+21,ystart+11,'~C~ONNECTiON',
                 instrument_bckg+instrument_glob,
                 instrument_bckg+instrument_hi_glob);

      If NOT ((vpos = 12) and (hpos = 3)) then
        ShowCStr(ptr_temp_screen,xstart+51,ystart+11,'~F~EEDBACK '+_feedb_4op,
                 instrument_bckg+instrument_glob,
                 instrument_bckg+instrument_hi_glob);
    end;

  If NOT ((vpos = 16) and (hpos = 2)) then
    ShowCStr(ptr_temp_screen,xstart+51,ystart+15,'F-~T~UNE '+_ftune_4op,
             instrument_bckg+instrument_glob,
             instrument_bckg+instrument_hi_glob);

  If NOT ((vpos = 17) and (hpos in [1..3])) then
    If carrier then
      ShowCStr(ptr_temp_screen,xstart+1,ystart+15,'~P~ANNiNG '+_pan_4op,
               instrument_bckg+instrument_car,
               instrument_bckg+instrument_hi_car)
    else ShowCStr(ptr_temp_screen,xstart+1,ystart+15,'~P~ANNiNG '+_pan_4op,
                  instrument_bckg+_col_mod,
                  instrument_bckg+_col_hi_mod);

  If NOT ((vpos in [18..21]) and (hpos = 1)) then
    If carrier then
      ShowCStr(ptr_temp_screen,xstart+1,ystart+19,'~E~NVELOPE TYPE',
               instrument_bckg+instrument_car,
               instrument_bckg+instrument_hi_car)
    else
      ShowCStr(ptr_temp_screen,xstart+1,ystart+19,'~E~NVELOPE TYPE',
               instrument_bckg+_col_mod,
               instrument_bckg+_col_hi_mod);

  If NOT ((vpos in [18..20]) and (hpos > 1)) then
    If carrier then
      ShowCStr(ptr_temp_screen,xstart+28,ystart+19,'FREQUENCY DATA ~M~ULTiPLiER',
               instrument_bckg+instrument_car,
               instrument_bckg+instrument_hi_car)
    else
      ShowCStr(ptr_temp_screen,xstart+28,ystart+19,'FREQUENCY DATA ~M~ULTiPLiER',
               instrument_bckg+_col_mod,
               instrument_bckg+_col_hi_mod);

  Case hpos of
    1: Case vpos of
         1:  If carrier then
               ShowStr(ptr_temp_screen,xstart+1,ystart+1,'ATTACK RATE',
                       instrument_bckg+instrument_hi_car)
             else
               ShowStr(ptr_temp_screen,xstart+1,ystart+1,'ATTACK RATE',
                       instrument_bckg+_col_hi_mod);

         2:  If carrier then
               ShowStr(ptr_temp_screen,xstart+1,ystart+2,'DECAY RATE',
                       instrument_bckg+instrument_hi_car)
             else
               ShowStr(ptr_temp_screen,xstart+1,ystart+2,'DECAY RATE',
                       instrument_bckg+_col_hi_mod);
         3,6,8,
         9:  If carrier then
               ShowStr(ptr_temp_screen,xstart+51,ystart+1,'WAVEFORM TYPE',
                       instrument_bckg+instrument_hi_car)
             else
               ShowStr(ptr_temp_screen,xstart+51,ystart+1,'WAVEFORM TYPE',
                       instrument_bckg+_col_hi_mod);

         4:  If carrier then
               ShowStr(ptr_temp_screen,xstart+1,ystart+4,'SUSTAiN LEVEL',
                       instrument_bckg+instrument_hi_car)
             else
               ShowStr(ptr_temp_screen,xstart+1,ystart+4,'SUSTAiN LEVEL',
                       instrument_bckg+_col_hi_mod);

         5:  If carrier then
               ShowStr(ptr_temp_screen,xstart+1,ystart+5,'RELEASE RATE',
                       instrument_bckg+instrument_hi_car)
             else
               ShowStr(ptr_temp_screen,xstart+1,ystart+5,'RELEASE RATE',
                       instrument_bckg+_col_hi_mod);

         7:  If carrier then
               ShowStr(ptr_temp_screen,xstart+1,ystart+7,'OUTPUT LEVEL',
                       instrument_bckg+instrument_hi_car)
             else
               ShowStr(ptr_temp_screen,xstart+1,ystart+7,'OUTPUT LEVEL',
                       instrument_bckg+_col_hi_mod);

         10..17: If (vpos in [10..13]) and (hpos = 1) then
                   If carrier then
                     ShowStr(ptr_temp_screen,xstart+1,ystart+9,'KEY SCALiNG LEVEL',
                             instrument_bckg+instrument_hi_car)
                   else
                     ShowStr(ptr_temp_screen,xstart+1,ystart+9,'KEY SCALiNG LEVEL',
                             instrument_bckg+_col_hi_mod)
                 else If (vpos <> 17) then
                        ShowStr(ptr_temp_screen,xstart+21,ystart+11,'CONNECTiON',
                                instrument_bckg+instrument_hi_glob)
                      else If carrier then
                             ShowStr(ptr_temp_screen,xstart+1,ystart+15,'PANNiNG '+_pan_4op,
                                     instrument_bckg+instrument_hi_car)
                           else
                             ShowStr(ptr_temp_screen,xstart+1,ystart+15,'PANNiNG '+_pan_4op,
                                     instrument_bckg+_col_hi_mod);
         18,19,20,
         21: If carrier then
               ShowStr(ptr_temp_screen,xstart+1,ystart+19,'ENVELOPE TYPE',
                       instrument_bckg+instrument_hi_car)
             else
               ShowStr(ptr_temp_screen,xstart+1,ystart+19,'ENVELOPE TYPE',
                       instrument_bckg+_col_hi_mod);
       end;

    2,3,4,5,6,
    7: Case vpos of
         2,4,5,
         7:  If carrier then
               ShowStr(ptr_temp_screen,xstart+51,ystart+1,'WAVEFORM TYPE',
                       instrument_bckg+instrument_hi_car)
             else
               ShowStr(ptr_temp_screen,xstart+51,ystart+1,'WAVEFORM TYPE',
                       instrument_bckg+_col_hi_mod);

         12..17:  If (vpos = 12) and (hpos = 3) then
                    ShowStr(ptr_temp_screen,xstart+51,ystart+11,'FEEDBACK '+_feedb_4op,
                            instrument_bckg+instrument_hi_glob)
                  else If (vpos = 16) and (hpos = 2) then
                         ShowStr(ptr_temp_screen,xstart+51,ystart+15,'F-TUNE '+_ftune_4op,
                                 instrument_bckg+instrument_hi_glob)
                       else If (vpos = 17) and (hpos in [1..3]) then
                              If carrier then
                                ShowStr(ptr_temp_screen,xstart+1,ystart+15,'PANNiNG '+_pan_4op,
                                        instrument_bckg+instrument_hi_car)
                              else ShowStr(ptr_temp_screen,xstart+1,ystart+15,'PANNiNG '+_pan_4op,
                                           instrument_bckg+_col_hi_mod)
                            else ShowStr(ptr_temp_screen,xstart+21,ystart+11,'CONNECTiON',
                                         instrument_bckg+instrument_hi_glob);
         18,19,20,
         21: If carrier then
               ShowStr(ptr_temp_screen,xstart+28,ystart+19,'FREQUENCY DATA MULTiPLiER',
                       instrument_bckg+instrument_hi_car)
             else
               ShowStr(ptr_temp_screen,xstart+28,ystart+19,'FREQUENCY DATA MULTiPLiER',
                       instrument_bckg+_col_hi_mod);
       end;
  end;

  _dbg_leave; //EXIT //(INSTRUMENT_CONTROL_edit)._page_build
end;
