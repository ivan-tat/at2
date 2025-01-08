// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure status_refresh;

var
  temps: String;
  temp,temp2,chan: Byte;
{$IFNDEF GO32V2}
  wav_indicator_attr: array[Boolean] of Byte;
{$ENDIF}
  playtime_attr1,playtime_attr2,
  playtime_attr3: array[Boolean] of Byte;
  bpm: Real;

begin
  _dbg_enter ({$I %FILE%}, 'status_refresh');
  _dbg_label ('1/4'); //status_refresh

  If really_no_status_refresh then
    begin
      _dbg_leave; EXIT; //status_refresh
    end;
  show_str(21,06,status_layout[play_status][8]+status_layout[play_status][9],
           status_background+status_play_state);

  _dbg_label ('2/4'); //status_refresh

  If no_status_refresh then
    begin
      chan := chan_pos;
      For temp2 := 1 to MAX_TRACKS do
        begin
          If channel_flag[chan] then
            show_str(08+(temp2-1)*15,MAX_PATTERN_ROWS+11,
                     ExpStrR('',14,#205),
                     pattern_bckg+pattern_border);
          Inc(chan);
        end;
      _dbg_leave; EXIT; //status_refresh
    end;

  _dbg_label ('3/4'); //status_refresh

  temps := status_layout[play_status];
  show_str(14,06,ExpStrR(temps,10,' '),
           status_background+status_play_state);
  If NOT (area_x1+area_y1 = 0) and (area_x1 <= 20) and (area_y1 <= 06) then
    show_str(02,01,' '+ExpStrR(temps,10,' '),
             main_background+main_border)
  else
    show_str(02,01,ExpStrR('',12,#196),
             main_background+main_border);

  If single_play then
    begin
      _dbg_leave; EXIT; //status_refresh
    end;

  _dbg_label ('4/4'); //status_refresh

  If play_single_patt then
    show_str(20,03,byte2hex(current_pattern)+'  ',
             pattern_bckg+status_dynamic_txt)
  else
    begin
      temp := songdata.pattern_order[current_order];
      If NOT (temp <= $7f) then temp := 0;
      If NOT replay_forbidden and repeat_pattern and
         NOT calibrating then
        show_str(20,03,byte2hex(temp)+' '#19,
                 pattern_bckg+status_dynamic_txt)
      else If NOT calibrating then
             show_str(20,03,byte2hex(temp)+'  ',
                      status_background+status_dynamic_txt)
           else show_str(20,03,byte2hex(temp),
                         status_background+status_dynamic_txt);
    end;

  If NOT play_single_patt then
    show_str(17,03,byte2hex(current_order),
             status_background+status_dynamic_txt)
  else show_str(17,03,'--',
                status_background+status_dynamic_txt);

  show_str(17,04,byte2hex(current_line),
           status_background+status_dynamic_txt);

  If _IRQFREQ_update_event and NOT (Addr(_show_bpm_realtime_proc) <> NIL) then
    begin
      bpm := calc_realtime_bpm_speed(tempo,speed,mark_line);
      show_str(03,05,'SPEED iN BPM',
               status_background+status_static_txt);
      show_str(17,05,ExpStrR(Bpm2str(bpm),7,' '),
               status_background+status_dynamic_txt)
    end
  else If NOT _IRQFREQ_update_event then
         begin
           show_str(03,05,'SPEED/TEMPO     /',
                    status_background+status_static_txt);
           show_str(17,05,byte2hex(speed),
                    status_background+status_dynamic_txt);
           If (tempo < 100) then
             If (tempo = 18) and timer_fix then
               show_str(20,05,byte2dec(tempo)+#5+#174,
                        status_background+status_dynamic_txt)
             else show_str(20,05,byte2dec(tempo)+#174+' ',
                           status_background+status_dynamic_txt)
           else show_str(20,05,byte2dec(tempo)+#174,
                         status_background+status_dynamic_txt);
         end;

  If (play_status <> isStopped) then
    temps := ' ~'+byte2dec(max(song_timer DIV 60,99))+':'+
                 byte2dec(max(song_timer MOD 60,99))+'.'+
                 CHR(48+max(song_timer_tenths DIV 10,9))+'~ '
  else temps := ' 00~:~00~.~0 ';

  playtime_attr1[FALSE] := status_background+status_text_dis;
  playtime_attr1[TRUE] := (status_border SHL 4) AND $0f0;
  playtime_attr2[FALSE] := status_background+status_border;
  playtime_attr2[TRUE] := status_background+status_border;
  playtime_attr3[FALSE] := main_background+main_border;
  playtime_attr3[TRUE] := (main_border SHL 4) AND $0f0;

  If _IRQFREQ_update_event then
    begin
      If (IRQ_freq_shift+playback_speed_shift > 0) then
        temps := '+'+Num2str(IRQ_freq_shift+playback_speed_shift,10)+#174
      else If (IRQ_freq_shift+playback_speed_shift < 0) then
             temps := '-'+Num2str(Abs(IRQ_freq_shift+playback_speed_shift),10)+#174
           else temps := Num2str(IRQ_freq+IRQ_freq_shift+playback_speed_shift,10)+#174;
      temps := '~ ~ '+CenterStR(temps,5)+' ~ ~';
    end;

  show_cstr(13,07,temps,
            playtime_attr1[_IRQFREQ_blink_flag AND _IRQFREQ_update_event],
            playtime_attr2[_IRQFREQ_blink_flag AND _IRQFREQ_update_event]);

  If NOT (area_x1+area_y1 = 0) and (area_x1 <= 20) and (area_y1 <= 06) then
    show_cstr(16,01,temps,
              playtime_attr3[_IRQFREQ_blink_flag AND _IRQFREQ_update_event],
              main_background+main_border)
  else
    show_str(16,01,ExpStrR('',11,#196),
             main_background+main_border);

  If NOT _IRQFREQ_blink_flag AND _IRQFREQ_update_event then
    _IRQFREQ_update_event := FALSE;

{$IFDEF GO32V2}
  temp := Pos('.',songdata_title);
  show_str(03,06,'['+ExpStrR(iCASE(Copy(songdata_title,1,PRED(temp))),8,' ')+']',
           status_background+status_dynamic_txt);
  show_str(03,07,'['+ExpStrR(iCASE(Copy(songdata_title,SUCC(temp),3)),3,' ')+']',
           status_background+status_border);

  If NOT module_archived then
    show_str(08,07,'['+#29+']',
             main_background+main_behavior)
  else show_str(08,07,#205#205#205,
                status_background+status_border);
{$ELSE}
  temps := BaseNameOnly(FilterStr2(songdata_title,_valid_characters_fname,'_'));

  While (8+_name_scrl_shift > Length(temps)) and (_name_scrl_shift > 0) do
    Dec(_name_scrl_shift);

  show_str(03,06,'['+ExpStrR(iCASE(Copy(temps,1+_name_scrl_shift,8)),8,' ')+']',
       status_background+status_dynamic_txt);
  show_str(03,07,'['+ExpStrR(iCASE(Copy(ExtOnly(songdata_title),1,3)),3,' ')+']',
       status_background+status_border);

  If (_name_scrl_pending_frames <> 0) then Dec(_name_scrl_pending_frames)
  else
    begin
      _name_scrl_pending_frames := 20;
      If (_name_scrl_shift_ctr > 0) then
        If (8+_name_scrl_shift < Length(temps)) then Inc(_name_scrl_shift,_name_scrl_shift_ctr)
        else begin
               _name_scrl_shift_ctr := -1;
               _name_scrl_pending_frames := _name_scrl_pending_frames*10;
             end
      else If (_name_scrl_shift > 0) then Inc(_name_scrl_shift,_name_scrl_shift_ctr)
           else begin
                  _name_scrl_shift_ctr := 1;
                  _name_scrl_pending_frames := _name_scrl_pending_frames*50;
                end;
    end;

  wav_indicator_attr[FALSE] := main_background+main_behavior;
  If (play_status = isPlaying) and NOT opl3_channel_recording_mode then
    wav_indicator_attr[TRUE] := (main_behavior SHL 4) AND $0f0
  else wav_indicator_attr[TRUE] := main_background+main_behavior;

  If NOT (area_x1+area_y1 = 0) and (area_x1 <= 20) and (area_y1 <= 06) then
    If (sdl_opl3_emulator <> 0) then
      show_str(13,01,'WAV',wav_indicator_attr[TRUE])
    else show_str(13,01,#196#196#196,main_background+main_border)
  else If (sdl_opl3_emulator = 1) then
         show_str(08,07,'[WAV]',wav_indicator_attr[_WAVREC_blink_flag])
       else If NOT module_archived then
              show_cstr(08,07,'~['+#29+']~'#205#205,
                        status_background+status_border,
                        main_background+main_behavior)
            else show_str(08,07,#205#205#205#205#205,
                          status_background+status_border);
{$ENDIF}

  If (command_typing = 1) then
    If NOT cycle_pattern then
      show_str(MAX_COLUMNS-17,MAX_PATTERN_ROWS+12,'A',main_background+main_behavior)
    else show_str(MAX_COLUMNS-17,MAX_PATTERN_ROWS+12,'F',main_background+main_behavior)
  else If (command_typing = 2) and NOT cycle_pattern then
         show_str(MAX_COLUMNS-17,MAX_PATTERN_ROWS+12,'S',main_background+main_behavior)
       else show_str(MAX_COLUMNS-17,MAX_PATTERN_ROWS+12,#196,main_background+main_border);

  If (songdata.flag_4op <> 0) then
    show_cstr(MAX_COLUMNS-43,MAX_PATTERN_ROWS+12,'~[~'+#4#3+'~]~',
              main_background+main_hi_stat_line,
              main_background+main_stat_line)
  else show_cstr(MAX_COLUMNS-43,MAX_PATTERN_ROWS+12,'~[~'+#2+#3+'~]~',
                main_background+main_hi_stat_line,
                main_background+main_stat_line);

  For temp := 1 to 8 do
    If (temp <> current_octave) then
      show_str(30+temp,MAX_PATTERN_ROWS+12,CHR(48+temp),
               main_background+main_stat_line)
    else show_str(30+temp,MAX_PATTERN_ROWS+12,CHR(48+temp),
                  main_background+main_hi_stat_line);

  chan := chan_pos;
  For temp2 := 1 to max(MAX_TRACKS,songdata.nm_tracks) do
    begin
      If NOT pan_lock[chan] or
         ((play_status = isStopped) and NOT debugging) then
        temp := pattern_bckg+pattern_pan_indic
      else temp := pattern_bckg+pattern_gpan_indic;

      If NOT ((play_status = isStopped) and NOT debugging and NOT panlock) and
         channel_flag[chan] then
        begin
          If percussion_mode and (chan in [16..20]) then
            Case chan of
              16: temps := '~BD~ ';
              17: temps := '~SD~ ';
              18: temps := '~TT~ ';
              19: temps := '~TC~ ';
              20: temps := '~HH~ ';
            end
          else Case chan of
                 1,
                 2:  If (songdata.flag_4op OR 1 = songdata.flag_4op) then
                       If NOT vol4op_lock[chan] then temps := '~'#4#3'~ '
                       else temps := '~'#4#3'+~'
                     else Case volslide_type[chan] of
                            0: temps := '   ';
                            1: temps := 'C'+#10+' ';
                            2: temps := 'M'+#10+' ';
                            3: temps := '&'+#10+' ';
                          end;
                 3,
                 4:  If (songdata.flag_4op OR 2 = songdata.flag_4op) then
                       If NOT vol4op_lock[chan] then temps := '~'#4#3'~ '
                       else temps := '~'#4#3'+~'
                     else Case volslide_type[chan] of
                            0: temps := '   ';
                            1: temps := 'C'+#10+' ';
                            2: temps := 'M'+#10+' ';
                            3: temps := '&'+#10+' ';
                          end;
                 5,
                 6:  If (songdata.flag_4op OR 4 = songdata.flag_4op) then
                       If NOT vol4op_lock[chan] then temps := '~'#4#3'~ '
                       else temps := '~'#4#3'+~'
                     else Case volslide_type[chan] of
                            0: temps := '   ';
                            1: temps := 'C'+#10+' ';
                            2: temps := 'M'+#10+' ';
                            3: temps := '&'+#10+' ';
                          end;
                 10,
                 11: If (songdata.flag_4op OR 8 = songdata.flag_4op) then
                       If NOT vol4op_lock[chan] then temps := '~'#4#3'~ '
                       else temps := '~'#4#3'+~'
                     else Case volslide_type[chan] of
                            0: temps := '   ';
                            1: temps := 'C'+#10+' ';
                            2: temps := 'M'+#10+' ';
                            3: temps := '&'+#10+' ';
                          end;
                 12,
                 13: If (songdata.flag_4op OR $10 = songdata.flag_4op) then
                       If NOT vol4op_lock[chan] then temps := '~'#4#3'~ '
                       else temps := '~'#4#3'+~'
                     else Case volslide_type[chan] of
                            0: temps := '   ';
                            1: temps := 'C'+#10+' ';
                            2: temps := 'M'+#10+' ';
                            3: temps := '&'+#10+' ';
                          end;
                 14,
                 15: If (songdata.flag_4op OR $20 = songdata.flag_4op) then
                       If NOT vol4op_lock[chan] then temps := '~'#4#3'~ '
                       else temps := '~'#4#3'+~'
                     else Case volslide_type[chan] of
                            0: temps := '   ';
                            1: temps := 'C'+#10+' ';
                            2: temps := 'M'+#10+' ';
                            3: temps := '&'+#10+' ';
                          end;

                 else Case volslide_type[chan] of
                        0: temps := '   ';
                        1: temps := 'C'+#10+' ';
                        2: temps := 'M'+#10+' ';
                        3: temps := '&'+#10+' ';
                      end;
               end;

          If volume_lock[chan] then temps := temps+'V+';
          If peak_lock[chan] then temps := temps+'P+';

          If panlock and (play_status = isStopped) and NOT debugging then
            Case songdata.lock_flags[chan] AND 3 of
              0: show_str(12+(temp2-1)*15,09,#21#22' ',temp);
              1: show_str(12+(temp2-1)*15,09,#21'  ',temp);
              2: show_str(12+(temp2-1)*15,09,' '#22' ',temp);
            end
          else
            Case panning_table[chan] of
              0: show_str(12+(temp2-1)*15,09,#21#22' ',temp);
              1: show_str(12+(temp2-1)*15,09,#21'  ',temp);
              2: show_str(12+(temp2-1)*15,09,' '#22' ',temp);
            end;

          If NOT (chan in [16..20]) then
            show_cstr(12+(temp2-1)*15+3,09,ExpStrR(temps,7,' '),
                      pattern_bckg+pattern_lock_indic,
                      pattern_bckg+pattern_4op_indic)
          else show_cstr(12+(temp2-1)*15+3,09,ExpStrR(temps,7,' '),
                         pattern_bckg+pattern_lock_indic,
                         pattern_bckg+pattern_perc_indic);
        end
      else If NOT percussion_mode and (chan in [16..20]) then
             show_str(12+(temp2-1)*15,09,#250#250' '+ExpStrL('',7,' '),
                      pattern_bckg+pattern_pan_indic)
           else Case chan of
                  1,
                  2:  If (songdata.flag_4op OR 1 = songdata.flag_4op) then
                        show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR(''#4#3'',7,' ')+'~',
                                  pattern_bckg+pattern_pan_indic,
                                  pattern_bckg+pattern_4op_indic)
                      else show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR('',7,' ')+'~',
                                     pattern_bckg+pattern_pan_indic,
                                     pattern_bckg+pattern_4op_indic);
                  3,
                  4:  If (songdata.flag_4op OR 2 = songdata.flag_4op) then
                        show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR(''#4#3'',7,' ')+'~',
                                  pattern_bckg+pattern_pan_indic,
                                  pattern_bckg+pattern_4op_indic)
                      else show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR('',7,' ')+'~',
                                     pattern_bckg+pattern_pan_indic,
                                     pattern_bckg+pattern_4op_indic);
                  5,
                  6:  If (songdata.flag_4op OR 4 = songdata.flag_4op) then
                        show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR(''#4#3'',7,' ')+'~',
                                  pattern_bckg+pattern_pan_indic,
                                  pattern_bckg+pattern_4op_indic)
                      else show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR('',7,' ')+'~',
                                     pattern_bckg+pattern_pan_indic,
                                     pattern_bckg+pattern_4op_indic);
                  10,
                  11: If (songdata.flag_4op OR 8 = songdata.flag_4op) then
                        show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR(''#4#3'',7,' ')+'~',
                                  pattern_bckg+pattern_pan_indic,
                                  pattern_bckg+pattern_4op_indic)
                      else show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR('',7,' ')+'~',
                                     pattern_bckg+pattern_pan_indic,
                                     pattern_bckg+pattern_4op_indic);
                  12,
                  13: If (songdata.flag_4op OR $10 = songdata.flag_4op) then
                        show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR(''#4#3'',7,' ')+'~',
                                  pattern_bckg+pattern_pan_indic,
                                  pattern_bckg+pattern_4op_indic)
                      else show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR('',7,' ')+'~',
                                     pattern_bckg+pattern_pan_indic,
                                     pattern_bckg+pattern_4op_indic);
                  14,
                  15: If (songdata.flag_4op OR $20 = songdata.flag_4op) then
                        show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR(''#4#3'',7,' ')+'~',
                                  pattern_bckg+pattern_pan_indic,
                                  pattern_bckg+pattern_4op_indic)
                      else show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR('',7,' ')+'~',
                                     pattern_bckg+pattern_pan_indic,
                                     pattern_bckg+pattern_4op_indic);

                  16: show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR('BD',7,' ')+'~',
                                pattern_bckg+pattern_pan_indic,
                                pattern_bckg+pattern_perc_indic);
                  17: show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR('SD',7,' ')+'~',
                                pattern_bckg+pattern_pan_indic,
                                pattern_bckg+pattern_perc_indic);
                  18: show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR('TT',7,' ')+'~',
                                pattern_bckg+pattern_pan_indic,
                                pattern_bckg+pattern_perc_indic);
                  19: show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR('TC',7,' ')+'~',
                                pattern_bckg+pattern_pan_indic,
                                pattern_bckg+pattern_perc_indic);
                  20: show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR('HH',7,' ')+'~',
                                pattern_bckg+pattern_pan_indic,
                                pattern_bckg+pattern_perc_indic);
                  else show_str(12+(temp2-1)*15,09,#250#250' '+ExpStrL('',7,' '),
                                pattern_bckg+pattern_pan_indic);
                end;

      If NOT channel_flag[chan] then
        begin
          show_str(08+(temp2-1)*15,MAX_PATTERN_ROWS+11,#205#205#205,
                   pattern_bckg+pattern_border);
          show_str(11+(temp2-1)*15,MAX_PATTERN_ROWS+11,' () OFF ',
                   pattern_bckg+pattern_chan_indic);
          If (temp2 < max(MAX_TRACKS,songdata.nm_tracks)) then
            show_str(19+(temp2-1)*15,MAX_PATTERN_ROWS+11,#205#205#205#207,
                     pattern_bckg+pattern_border)
          else
            show_str(19+(temp2-1)*15,MAX_PATTERN_ROWS+11,#205#205#205#202,
                     pattern_bckg+pattern_border);

          If NOT percussion_mode and (chan in [16..20]) then
            show_str(12+(temp2-1)*15,09,#250#250' '+ExpStrL('',7,' '),
                     pattern_bckg+pattern_pan_indic)
          else Case chan of
                 1,
                 2:  If (songdata.flag_4op OR 1 = songdata.flag_4op) then
                       show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR(''#4#3'',7,' ')+'~',
                                 pattern_bckg+pattern_pan_indic,
                                 pattern_bckg+pattern_4op_indic)
                     else show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR('',7,' ')+'~',
                                    pattern_bckg+pattern_pan_indic,
                                    pattern_bckg+pattern_4op_indic);
                 3,
                 4:  If (songdata.flag_4op OR 2 = songdata.flag_4op) then
                       show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR(''#4#3'',7,' ')+'~',
                                 pattern_bckg+pattern_pan_indic,
                                 pattern_bckg+pattern_4op_indic)
                     else show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR('',7,' ')+'~',
                                    pattern_bckg+pattern_pan_indic,
                                    pattern_bckg+pattern_4op_indic);
                 5,
                 6:  If (songdata.flag_4op OR 4 = songdata.flag_4op) then
                       show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR(''#4#3'',7,' ')+'~',
                                 pattern_bckg+pattern_pan_indic,
                                 pattern_bckg+pattern_4op_indic)
                     else show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR('',7,' ')+'~',
                                    pattern_bckg+pattern_pan_indic,
                                    pattern_bckg+pattern_4op_indic);
                 10,
                 11: If (songdata.flag_4op OR 8 = songdata.flag_4op) then
                       show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR(''#4#3'',7,' ')+'~',
                                 pattern_bckg+pattern_pan_indic,
                                 pattern_bckg+pattern_4op_indic)
                     else show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR('',7,' ')+'~',
                                    pattern_bckg+pattern_pan_indic,
                                    pattern_bckg+pattern_4op_indic);
                 12,
                 13: If (songdata.flag_4op OR $10 = songdata.flag_4op) then
                       show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR(''#4#3'',7,' ')+'~',
                                 pattern_bckg+pattern_pan_indic,
                                 pattern_bckg+pattern_4op_indic)
                     else show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR('',7,' ')+'~',
                                    pattern_bckg+pattern_pan_indic,
                                    pattern_bckg+pattern_4op_indic);
                 14,
                 15: If (songdata.flag_4op OR $20 = songdata.flag_4op) then
                       show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR(''#4#3'',7,' ')+'~',
                                 pattern_bckg+pattern_pan_indic,
                                 pattern_bckg+pattern_4op_indic)
                     else show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR('',7,' ')+'~',
                                    pattern_bckg+pattern_pan_indic,
                                    pattern_bckg+pattern_4op_indic);

                 16: show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR('BD',7,' ')+'~',
                               pattern_bckg+pattern_pan_indic,
                               pattern_bckg+pattern_perc_indic);
                 17: show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR('SD',7,' ')+'~',
                               pattern_bckg+pattern_pan_indic,
                               pattern_bckg+pattern_perc_indic);
                 18: show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR('TT',7,' ')+'~',
                               pattern_bckg+pattern_pan_indic,
                               pattern_bckg+pattern_perc_indic);
                 19: show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR('TC',7,' ')+'~',
                               pattern_bckg+pattern_pan_indic,
                               pattern_bckg+pattern_perc_indic);
                 20: show_cstr(12+(temp2-1)*15,09,#250#250' ~'+ExpStrR('HH',7,' ')+'~',
                               pattern_bckg+pattern_pan_indic,
                               pattern_bckg+pattern_perc_indic);
                 else show_str(12+(temp2-1)*15,09,#250#250' '+ExpStrL('',7,' '),
                               pattern_bckg+pattern_pan_indic);
               end;
        end
      else begin
             temp := 0;
{$IFDEF GO32V2}
             If NOT ((scr_scroll_y > 10) or NOT is_scrollable_screen_mode) then
{$ELSE}
             If NOT ((screen_scroll_offset > 10) or NOT is_default_screen_mode) then
{$ENDIF}
               If NOT is_4op_chan(chan) or
                  (is_4op_chan(chan) and (((chan in _4op_tracks_lo) and (temp2 = 1)) or
                                          ((chan in _4op_tracks_hi) and (temp2 = max(MAX_TRACKS,songdata.nm_tracks))))) then
                 begin
                   temp := Round(42/63*volum_bar[chan].lvl);
                   If (temp2 < max(MAX_TRACKS,songdata.nm_tracks)) then
                     show_str(08+(temp2-1)*15,11+MAX_PATTERN_ROWS,
                              ExpStrR(_vol_bar_str(temp),14,#205)+#207,
                              pattern_bckg+pattern_border)
                   else
                     show_str(08+(temp2-1)*15,11+MAX_PATTERN_ROWS,
                              ExpStrR(_vol_bar_str(temp),14,#205)+#202,
                              pattern_bckg+pattern_border);
                 end
               else If chan in _4op_tracks_hi then
                      begin
                        temp := Round(84/63*volum_bar[chan].lvl);
                        temps := _vol_bar_str(temp);
                        If (Length(temps) > 14) then
                          begin
                            temps[15] := #169;
                            temps := ExpStrR(temps,29,#205);
                          end
                        else
                          begin
                            temps := ExpStrR(temps,29,#205);
                            temps[15] := #207;
                          end;
                        If (temp2 < PRED(max(MAX_TRACKS,songdata.nm_tracks))) then
                          temps := temps+#207
                        else temps := temps+#202;
                        show_str(08+(temp2-1)*15,11+MAX_PATTERN_ROWS,
                                 temps,
                                 pattern_bckg+pattern_border);
                      end
                    else
             else
               show_str(08+(temp2-1)*15,11+MAX_PATTERN_ROWS,Copy(patt_win[5],07+(temp2-1)*15,15),
                        pattern_bckg+pattern_border);
           end;
      Inc(chan);
    end;

  _dbg_leave; //EXIT //status_refresh
end;
