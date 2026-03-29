// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure fix_fmk_commands(patterns: Byte);

var
  chunk,chunk2,
  chunk3: tCHUNK;
  patt_break: Byte;
  order,patt: Byte;
  patts: String;
  ins_cache,
  misc_cache,
  arpg_cache,
  forcevol_cache,
  volsld_cache,
  xfvolsld_cache,
  slide_cache: array[1..20] of Byte;
  _1st_ins_load: array[1..20] of Boolean;
  _speed_table_fixed: array[0..$7f] of Boolean;
  prev_cache: array[1..20] of Record
                                effect_def,
                                effect,
                                effect_def2,
                                effect2: Byte;
                              end;

procedure fix_single_pattern(patt: Byte);

var
  temp2,temp3: Byte;

begin
  FillChar(prev_cache,SizeOf(prev_cache),0);
  patt_break := BYTE_NULL;

  If NOT _speed_table_fixed[patt] then
    For temp2 := 0 to $3f do
      begin
        For temp3 := 1 to 20 do
          begin
            get_chunk(patt,temp2,temp3,chunk);
            If (chunk.effect_def = 0) then
              begin
                chunk.effect_def := ef_SetCustomSpeedTab;
                chunk.effect := $0fa;
                put_chunk(patt,temp2,temp3,chunk);
                _speed_table_fixed[patt] := TRUE;
              end
            else If (chunk.effect_def2 = 0) then
                   begin
                     chunk.effect_def2 := ef_SetCustomSpeedTab;
                     chunk.effect2 := $0fa;
                     put_chunk(patt,temp2,temp3,chunk);
                     _speed_table_fixed[patt] := TRUE;
                   end;
            If _speed_table_fixed[patt] then BREAK;
          end;
        If _speed_table_fixed[patt] then BREAK;
      end;

  For temp2 := 0 to $3f do
    For temp3 := 1 to 20 do
      begin
        get_chunk(patt,temp2,temp3,chunk);
        If (chunk.effect_def = temp_ef_rep) then
          begin
            chunk.effect_def := prev_cache[temp3].effect_def;
            put_chunk(patt,temp2,temp3,chunk);
          end;

        If (chunk.effect_def = temp_ef_XFVSlide) then
          begin
            chunk.effect_def := ef_Extended2;
            If (xfvolsld_cache[temp3] <> 0) then
              chunk.effect := ef_ex2_VolSlideUpXF*16+volsld_cache[temp3] DIV 16
            else chunk.effect := ef_ex2_VolSlideDnXF*16+volsld_cache[temp3] MOD 16;
            put_chunk(patt,temp2,temp3,chunk);
          end;

        If (chunk.effect_def in [ef_PositionJump,ef_PatternBreak]) then
          patt_break := temp2;

        If (temp2 <= patt_break) and
           (chunk.instr_def <> ins_cache[temp3]) and
           (chunk.effect_def2 <> ef_ForceInsVolume) then
          If (chunk.instr_def <> 0) then
            forcevol_cache[temp3] := 0;

        If ((chunk.effect_def = ef_Extended) and
            (chunk.effect = ef_ex_ExtendedCmd2*16+ef_ex_cmd2_ResetVol)) or
           ((chunk.effect_def2 = ef_Extended) and
            (chunk.effect2 = ef_ex_ExtendedCmd2*16+ef_ex_cmd2_ResetVol)) then
          forcevol_cache[temp3] := 0;

        If (chunk.effect_def2 = ef_ForceInsVolume) and
           (temp2 <= patt_break) then
          forcevol_cache[temp3] := 1;

        If (chunk.instr_def <> 0) and (temp2 <= patt_break) then
          ins_cache[temp3] := chunk.instr_def;

        If (chunk.instr_def <> 0) or ((chunk.instr_def = 0) and
                                      (chunk.note in [1..12*8+1]) and
                                      (ins_cache[temp3] <> 0)) then
          put_chunk(patt,temp2,temp3,chunk);

        If (temp2 <= patt_break) then
          begin
            If (chunk.effect DIV 16 <> 0) then
              misc_cache[temp3] := chunk.effect AND $0f0+
                                   misc_cache[temp3] AND $0f
            else If (chunk.effect_def in [ef_Vibrato,
                                          ef_Tremolo]) then
                   begin
                     chunk.effect := misc_cache[temp3] AND $0f0+
                                     chunk.effect AND $0f;
                     put_chunk(patt,temp2,temp3,chunk);
                   end;

            If (chunk.effect MOD 16 <> 0) then
              misc_cache[temp3] := misc_cache[temp3] AND $0f0+
                                   chunk.effect AND $0f
            else If (chunk.effect_def in [ef_Vibrato,
                                          ef_Tremolo]) then
                   begin
                     chunk.effect := chunk.effect AND $0f0+
                                     misc_cache[temp3] AND $0f;
                     put_chunk(patt,temp2,temp3,chunk);
                   end;

            If (chunk.effect_def = ef_RetrigNote) then
              If (chunk.effect <> 0) then misc_cache[temp3] := chunk.effect
              else begin
                     chunk.effect := misc_cache[temp3];
                     put_chunk(patt,temp2,temp3,chunk);
                   end;

            If (chunk.effect_def = temp_ef_Arpeggio) then
              If (chunk.effect <> 0) then arpg_cache[temp3] := chunk.effect
              else begin
                     chunk.effect := arpg_cache[temp3];
                     put_chunk(patt,temp2,temp3,chunk);
                   end;

            If (chunk.effect_def in [ef_FSlideDown,ef_FSlideDownFine,
                                     ef_FSlideUp,ef_FSlideUpFine,
                                     ef_TonePortamento]) then
              If (chunk.effect <> 0) then slide_cache[temp3] := chunk.effect
              else begin
                     chunk.effect := slide_cache[temp3];
                     put_chunk(patt,temp2,temp3,chunk);
                   end;

            If (chunk.effect_def = ef_Extended2) and
               (chunk.effect DIV 16 in [ef_ex2_FreqSlideDnXF,ef_ex2_FreqSlideUpXF]) then
              If (chunk.effect MOD 16 <> 0) then slide_cache[temp3] := chunk.effect MOD 16
              else begin
                     chunk.effect := chunk.effect AND $0f0+slide_cache[temp3] AND $0f;
                     put_chunk(patt,temp2,temp3,chunk);
                   end;

            If (chunk.effect_def in [ef_TPortamVolSlide,ef_VibratoVolSlide,
                                     ef_VolSlide,ef_VolSlideFine]) and
               (temp2 <= patt_break) then
              begin
                If (chunk.effect <> 0) then volsld_cache[temp3] := chunk.effect
                else begin
                       chunk.effect := volsld_cache[temp3];;
                       put_chunk(patt,temp2,temp3,chunk);
                     end;
              end;

            If (chunk.effect_def = ef_Extended2) and
               (chunk.effect DIV 16 in [ef_ex2_VolSlideDnXF,ef_ex2_VolSlideUpXF]) then
              If (chunk.effect MOD 16 <> 0) then
                Case chunk.effect DIV 16 of
                  ef_ex2_VolSlideDnXF:
                    begin
                      volsld_cache[temp3] := chunk.effect MOD 16;
                      xfvolsld_cache[temp3] := 0;
                    end;

                  ef_ex2_VolSlideUpXF:
                    begin
                      volsld_cache[temp3] := chunk.effect MOD 16*16;
                      xfvolsld_cache[temp3] := 1;
                    end;
                end;
          end;

        If (prev_cache[temp3].effect_def in [ef_Vibrato,ef_VibratoVolSlide]) and
           NOT (chunk.effect_def in [ef_Vibrato,ef_VibratoVolSlide]) then
          If (chunk.effect_def = 0) and (chunk.effect = 0) then
            begin
              chunk2 := chunk;
              chunk2.effect_def := ef_Extended;
              chunk2.effect := ef_ex_ExtendedCmd2*16+ef_ex_cmd2_VibrOff;
              If NOT ((chunk2.effect_def = chunk2.effect_def2) and
                      (chunk2.effect = chunk2.effect2)) then
                begin
                  put_chunk(patt,temp2,temp3,chunk2);
                  chunk := chunk2;
                end;
            end
          else If (chunk.effect_def2 = 0) and (chunk.effect2 = 0) then
                 begin
                   chunk2 := chunk;
                   chunk2.effect_def2 := ef_Extended;
                   chunk2.effect2 := ef_ex_ExtendedCmd2*16+ef_ex_cmd2_VibrOff;
                   If NOT ((chunk2.effect_def2 = chunk2.effect_def) and
                           (chunk2.effect2 = chunk2.effect)) then
                     begin
                       put_chunk(patt,temp2,temp3,chunk2);
                       chunk := chunk2;
                     end;
                 end;

        If (_1st_ins_load[temp3] and (chunk.instr_def <> 0)) or
           (forcevol_cache[temp3] <> 0) and
           (temp2 <= patt_break) and
           (chunk.instr_def <> 0) then
          If (chunk.effect_def2+chunk.effect2 = 0) then
            If NOT (chunk.effect_def in [ef_SetModulatorVol,ef_SetCarrierVol]) then
              begin
                chunk.effect_def2 := ef_Extended;
                chunk.effect2 := ef_ex_ExtendedCmd2*16+ef_ex_cmd2_ResetVol;
                put_chunk(patt,temp2,temp3,chunk);
                forcevol_cache[temp3] := 0;
                _1st_ins_load[temp3] := FALSE;
              end
            else begin
                  chunk.effect_def2 := chunk.effect_def;
                  chunk.effect2 := chunk.effect;
                  chunk.effect_def := ef_Extended;
                  chunk.effect := ef_ex_ExtendedCmd2*16+ef_ex_cmd2_ResetVol;
                  put_chunk(patt,temp2,temp3,chunk);
                  forcevol_cache[temp3] := 0;
                  _1st_ins_load[temp3] := FALSE;
                end;

        prev_cache[temp3].effect_def := chunk.effect_def;
        prev_cache[temp3].effect := chunk.effect;
        prev_cache[temp3].effect_def2 := chunk.effect_def2;
        prev_cache[temp3].effect2 := chunk.effect2;

        If is_4op_chan(temp3) and
           (temp3 in _4op_tracks_hi) then
          begin
            get_chunk(patt,temp2,SUCC(temp3),chunk3);
            If (chunk.instr_def = 0) and (chunk3.instr_def <> 0) then
              begin
                If (ins_cache[temp3] <> 0) then
                  chunk.instr_def := ins_cache[temp3]
                else chunk.instr_def := chunk3.instr_def;
                put_chunk(patt,temp2,temp3,chunk);
              end;
          end;

        If (chunk.effect_def = temp_ef_Arpeggio) then
          begin
            chunk2 := chunk;
            chunk2.effect_def := ef_Arpeggio;
            put_chunk(patt,temp2,temp3,chunk2);
          end;

        If (chunk.effect_def in [ef_SetModulatorVol,ef_SetCarrierVol]) and
           (chunk.effect_def2 = ef_ForceInsVolume) then
          begin
            chunk2 := chunk;
            chunk2.effect_def := chunk.effect_def2;
            chunk2.effect := chunk.effect2;
            chunk2.effect_def2 := chunk.effect_def;
            chunk2.effect2 := chunk.effect;
            put_chunk(patt,temp2,temp3,chunk2);
          end;
      end;
end;

begin { fix_fmk_commands }
  FillChar(ins_cache,SizeOf(ins_cache),0);
  FillChar(_1st_ins_load,SizeOf(_1st_ins_load),TRUE);
  FillChar(_speed_table_fixed,SizeOf(_speed_table_fixed),FALSE);
  FillChar(xfvolsld_cache,SizeOf(volsld_cache),0);
  FillChar(volsld_cache,SizeOf(volsld_cache),0);
  FillChar(slide_cache,SizeOf(slide_cache),0);
  FillChar(misc_cache,SizeOf(misc_cache),0);
  FillChar(arpg_cache,SizeOf(arpg_cache),0);
  FillChar(forcevol_cache,SizeOf(forcevol_cache),0);

  patts := '';
  order := 0; patt := BYTE_NULL;

  Repeat
    If (songdata.pattern_order[order] >= $80) then Inc(order)
    else
      begin
        patt := songdata.pattern_order[order];
        fix_single_pattern(patt);
        Inc(order);
        patts := patts+CHR(patt);
      end;
  until (patt >= patterns) or (order > $7f);

  For patt := 0 to PRED(patterns) do
    If NOT (Pos(CHR(patt),patts) <> 0) then
      fix_single_pattern(patt);
end;

procedure fmk_file_loader;

type
  tFMK_HEADER = Record
                  id:       array[1..4] of Char; { FMK! }
                  songname: array[1..28] of Char; { Song name (28) }
                  composer: array[1..28] of Char; { Composer name (28) }
                  bytef4:   Byte; { Value 244 (f4h), just for check. }
                  ftype:    Byte; { File type {1=evolution 1, 2=evolution 2 }
                  glob_var: Byte; { Global variables, bits : 0 = stereo, 1 = opl3, 2 = rhythm }
                                  { 3 = 4.8 db tremolo  4 = 14 cent vibrato. }
                  base_spd: Byte; { Song basespeed, ticks / second. this version : fixed 50. }
                  init_spd: Byte; { Song initial speed. }
                  reserved: array[0..8] of Byte; { Reserved }
                  ordnum:   Byte; { Length of song (order). }
                  insnum:   Byte; { Number of instruments. }
                  patnum:   Byte; { Number of patterns. }
                  trk_pan:  array[1..5] of Byte; { Track stereo pan positions, bits 0-1, 2-3, 4-5, 6-7. }
                                                 { value 0 = left 1 = both 2 = right, from track 1 to 18. }
                  trk_set:  array[1..20] of Byte; { Track initial settings, 255=unused, bits : }
                                                  { 0-2, type value: 0 = normal 1=hihat 2=cymbal 3=tom tom 4=snare 5=bass }
                                                  {                  6 = 4op    7=unused }
                                                  { 3-7, OPL-channel number (1-18), 21 = none. }
{ ### if ftype=2 --> trk_set: 1..18; type_value: 0 = normal  6 = 4op  7=unused }
                end;
const
  id = 'FMK!';

const
  _conv_fmk_pan: array[0..2] of Byte = (1,0,2);

type
  tFIN_DATA = Record
                dname: array[1..12] of Char;
                iname: array[1..27] of Char;
                idata: tFM_INST_DATA;
              end;
var
  f: File;
  header: tFMK_HEADER;
  order_list: array[0..254] of Byte;
  paraptr_ins: array[1..99] of Word;
  paraptr_pat: array[0..63] of Longint;
  paraptr_msg: Word;
  insdata: tFIN_DATA;
  temp,temp2,fpos_bak: Longint;
  pat,row,chan,
  desc_rows: Byte;
  note,ins,vol,cmd,info: Byte;
  patlen,index: Word;
  dscbuf: array[0..PRED(20*24)] of Char;

{$IFNDEF ADT2PLAY}
procedure list_fmk_description(rows: Byte);

var
  desc: array[1..24] of String[20];
  temp,fkey: Word;
  row,temp2: Byte;
  xstart,ystart: Byte;

begin
  FillChar(desc,SizeOf(desc),0);

  temp := 0;
  For row := 1 to rows do
    begin
      desc[row] := '';
      For temp2 := 1 to 20 do
        begin
          desc[row] := desc[row]+dscbuf[temp];
          Inc(temp);
        end;
    end;

  temp2 := 0;
  For temp := 1 to rows do
    If (truncate_string(desc[temp]) <> '') then Inc(temp2);
  If (temp2 = 0) then
    EXIT; //list_fmk_description

  ScreenMemCopy(screen_ptr,ptr_screen_backup);
  centered_frame_vdest := screen_ptr;
  centered_frame(xstart,ystart,21,26,' FMK DESCRiPTiON ',
                 dialog_background+dialog_border,
                 dialog_background+dialog_title,
                 frame_double);

  For temp := 1 to rows do
    ShowStr(screen_ptr,xstart+1,ystart+temp,FilterStr2(desc[temp],_valid_characters,'_'),
            dialog_background+dialog_context_dis);

  ShowCStr(screen_ptr,xstart+6,ystart+25,' ~C~ONTiNUE ',
           dialog_sel_itm_bck+dialog_sel_itm,
           dialog_sel_itm_bck+dialog_sel_short);

  Repeat fkey := GetKey;
  until (fkey = kESC) or (fkey = kENTER) or (HI(fkey) = $2e);

  move_to_screen_data := ptr_screen_backup;
  move_to_screen_area[1] := xstart;
  move_to_screen_area[2] := ystart;
  move_to_screen_area[3] := xstart+21+2;
  move_to_screen_area[4] := ystart+26+1;
  move2screen;
  no_status_refresh := TRUE;

  //EXIT //list_fmk_description
end;
{$ENDIF}

procedure import_fmk_event(pattern,line,channel,note,ins,vol,cmd,info: Byte);

var
  chunk: tCHUNK;

begin
  FillChar(chunk,SizeOf(chunk),0);
  If (ins in [1..99]) then chunk.instr_def := ins;

  Case note of
    254: chunk.note := BYTE_NULL;
    255: chunk.note := 0;
    else If (note AND $0f in [1..12]) then
           chunk.note := 12*(note SHR 4)+(note AND $0f)
  end;

  If (vol <> BYTE_NULL) then
    begin
      chunk.effect_def2 := ef_ForceInsVolume;
      chunk.effect2 := 63-max(vol,63)
    end;

  Case CHR(cmd+ORD('A')-1) of
  { SET SPEED }
    'A': If (info <> 0) then
           begin
             chunk.effect_def := ef_SetSpeed;
             chunk.effect := info;
           end;

  { JUMP TO ORDER }
    'B': If (info <= 254) then
           begin
             chunk.effect_def := ef_PositionJump;
             chunk.effect := info;
           end;

  { CARRIER PARAM }
    'C': Case info DIV 16 of
           1: begin
                chunk.effect_def := ef_Extended3;
                chunk.effect := ef_ex3_SetMultipC*16+info MOD 16;
              end;

           2: begin
                chunk.effect_def := ef_Extended3;
                chunk.effect := ef_ex3_SetKslC*16+(info MOD 16) AND 3;
              end;

           3: begin
                chunk.effect_def := ef_Extended;
                chunk.effect := ef_ex_SetAttckRateC*16+info MOD 16;
              end;

           4: begin
                chunk.effect_def := ef_Extended;
                chunk.effect := ef_ex_SetDecayRateC*16+info MOD 16;
              end;

           5: begin
                chunk.effect_def := ef_Extended;
                chunk.effect := ef_ex_SetSustnLevelC*16+info MOD 16;
              end;

           6: begin
                chunk.effect_def := ef_Extended;
                chunk.effect := ef_ex_SetRelRateC*16+info MOD 16;
              end;

           7: begin
                chunk.effect_def := ef_SetWaveform;
                chunk.effect := info AND 7 SHL 4+$0f;
              end;

           8: begin
                chunk.effect_def := ef_Extended;
                chunk.effect := ef_ex_SetFeedback*16+info AND 7;
              end;
         end;

  { VOLUME SLIDE }
    'D': { VOLUME SLIDE DOWN }
         Case info DIV 16 of
         { NORMAL }
           0: If (info MOD 16 = 0) then chunk.effect_def := temp_ef_XFVSlide
              else begin
                     chunk.effect_def := ef_Extended2;
                     chunk.effect := ef_ex2_VolSlideDnXF*16+info MOD 16
                   end;
         { FINE }
          15: begin
                chunk.effect_def := ef_VolSlideFine;
                chunk.effect := info MOD 16;
              end;
         else
           { VOLUME SLIDE UP }
           Case info MOD 16 of
           { NORMAL }
             0: If (info DIV 16 = 0) then chunk.effect_def := temp_ef_XFVSlide
                else begin
                       chunk.effect_def := ef_Extended2;
                       chunk.effect := ef_ex2_VolSlideUpXF*16+info DIV 16;
                     end;
           { FINE }
            15: begin
                  chunk.effect_def := ef_VolSlideFine;
                  chunk.effect := info AND $0f0;
                end;
           end;
         end;

  { SLIDE DOWN }
    'E': Case info DIV 16 of
         { NORMAL }
           0..14: begin
                    chunk.effect_def := ef_FSlideDown;
                    chunk.effect := info;
               end;

         { FINE }
           15: begin
                 chunk.effect_def := ef_FSlideDownFine;
                 chunk.effect := info AND $0f;
               end;
         end;

  { SLIDE UP }
    'F': Case info DIV 16 of
         { NORMAL }
           0..14: begin
                    chunk.effect_def := ef_FSlideUp;
                    chunk.effect := info;
                  end;

         { FINE }
           15: begin
                 chunk.effect_def := ef_FSlideUpFine;
                 chunk.effect := info AND $0f;
               end;
         end;

  { TONE PORTAMENTO }
    'G': begin
           chunk.effect_def := ef_TonePortamento;
           chunk.effect := info;
         end;

  { VIBRATO }
    'H': begin
           chunk.effect_def := ef_Vibrato;
           If (info <> 0) and (info DIV 16 = 0) then
             chunk.effect := $10+info AND $0f
           else If (info <> 0) and (info MOD 16 = 0) then
                  chunk.effect := info AND $0f0+1
                else chunk.effect := info;
         end;

  { RETRIG NOTE }
    'I': begin
           chunk.effect_def := ef_RetrigNote;
           If (info <> 0) then chunk.effect := max(info*2,255);
         end;

  { ARPEGGIO }
    'J': begin
           chunk.effect_def := temp_ef_Arpeggio;
           chunk.effect := info;
         end;

  { MODLATOR PARAM }
    'M': Case info DIV 16 of
           1: begin
                chunk.effect_def := ef_Extended3;
                chunk.effect := ef_ex3_SetMultipM*16+info MOD 16;
              end;

           2: begin
                chunk.effect_def := ef_Extended3;
                chunk.effect := ef_ex3_SetKslM*16+(info MOD 16) AND 3;
              end;

           3: begin
                chunk.effect_def := ef_Extended;
                chunk.effect := ef_ex_SetAttckRateM*16+info MOD 16;
              end;

           4: begin
                chunk.effect_def := ef_Extended;
                chunk.effect := ef_ex_SetDecayRateM*16+info MOD 16;
              end;

           5: begin
                chunk.effect_def := ef_Extended;
                chunk.effect := ef_ex_SetSustnLevelM*16+info MOD 16;
              end;

           6: begin
                chunk.effect_def := ef_Extended;
                chunk.effect := ef_ex_SetRelRateM*16+info MOD 16;
              end;

           7: begin
                chunk.effect_def := ef_SetWaveform;
                chunk.effect := $0f0+info AND 7;
              end;

           8: begin
                chunk.effect_def := ef_Extended;
                chunk.effect := ef_ex_SetFeedback*16+info AND 7;
              end;
         end;

  { SET VIBRATO/TREMOLO WAVEFORM }
    'N': ;

  { BREAK PATTERN }
    'P': If (info < 64) then
           begin
             chunk.effect_def := ef_PatternBreak;
             chunk.effect := Str2num(Num2str(info,16),10);
           end;

  { TREMOLO }
    'R': begin
           chunk.effect_def := ef_Tremolo;
           If (info <> 0) and (info DIV 16 = 0) then
             chunk.effect := $10+info AND $0f
           else If (info <> 0) and (info MOD 16 = 0) then
                  chunk.effect := info AND $0f0+1
                else chunk.effect := info;
         end;

  { STEREO CONTROL }
    'S': If (header.glob_var AND 1 = 1) then
           begin
             chunk.effect_def := ef_Extended;
             Case info of
               1: chunk.effect := ef_ex_SetPanningPos*16+1;
               2: chunk.effect := ef_ex_SetPanningPos*16+0;
               3: chunk.effect := ef_ex_SetPanningPos*16+2;
             end;
           end;

  { MODULATOR VOLUME }
    'T': begin
           chunk.effect_def := ef_SetModulatorVol;
           chunk.effect := info AND $3f;
         end;

  { CARRIER VOLUME }
    'U': begin
           chunk.effect_def := ef_SetCarrierVol;
           chunk.effect := info AND $3f;
         end;
  end;

  If (chunk.effect_def = 0) and (chunk.effect <> 0) then
    chunk.effect := 0;
  put_chunk(pattern,line,channel,chunk);
end;

begin
  _dbg_enter ({$I %FILE%}, 'fmk_file_loader');

  {$i-}
  Assign(f,songdata_source);
  ResetF(f);
  {$i+}
  If (IOresult <> 0) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('ERROR READiNG DATA - DiSK ERROR?$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' FMK LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //fmk_file_loader
    end;

  BlockReadF(f,header,SizeOf(header),temp);
  If NOT ((temp = SizeOf(header)) and (header.id = id)) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' FMK LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //fmk_file_loader
    end;

{$IFDEF ADT2PLAY}
  load_flag := $7f;
{$ENDIF}
  If (header.bytef4 <> $f4) or NOT (header.ftype in [1,2]) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('UNKNOWN FiLE FORMAT TYPE$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' FMK LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //fmk_file_loader
    end;

{$IFNDEF ADT2PLAY}
  If (play_status <> isStopped) then
    begin
      fade_out_playback(FALSE);
      stop_playing;
    end;
{$ENDIF}

  If (header.ftype = 2) then
    begin
      SeekF(f,SizeOf(header)-2);
      If (IOresult <> 0) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' FMK LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //fmk_file_loader
        end;
    end;

  If (header.ordnum <> 0) then
    begin
      BlockReadF(f,order_list,header.ordnum,temp);
      If (IOresult <> 0) or (temp <> header.ordnum) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' FMK LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //fmk_file_loader
        end;
    end;

  BlockReadF(f,paraptr_msg,SizeOf(paraptr_msg),temp);
  If (IOresult <> 0) or (temp <> SizeOf(paraptr_msg)) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' FMK LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //fmk_file_loader
    end;

  fpos_bak := FilePos(f);
  If (paraptr_msg <> 0) then
    begin
      SeekF(f,paraptr_msg);
      If (IOresult <> 0) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' FMK LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //fmk_file_loader
        end;

      BlockReadF(f,desc_rows,SizeOf(desc_rows),temp);
      If (IOresult <> 0) or (temp <> SizeOf(desc_rows)) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' FMK LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //fmk_file_loader
        end;

      If (desc_rows <> 0) then
        begin
          BlockReadF(f,dscbuf,desc_rows*20,temp);
          If (IOresult <> 0) or (temp <> desc_rows*20) then
            begin
              CloseF(f);
{$IFNDEF ADT2PLAY}
              Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                     'LOADiNG STOPPED$',
                     '~O~KAY$',' FMK LOADER ',1);
{$ENDIF}
              _dbg_leave; EXIT; //fmk_file_loader
            end;
        end;

{$IFNDEF ADT2PLAY}
     If mod_description and (desc_rows <> 0) and NOT quick_cmd and
        NOT shift_pressed then
       list_fmk_description(desc_rows);
{$ENDIF}
    end;

  SeekF(f,fpos_bak);
  If (IOresult <> 0) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' FMK LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //fmk_file_loader
    end;

  If (header.insnum <> 0) then
    begin
      BlockReadF(f,paraptr_ins,header.insnum*2,temp);
      If (IOresult <> 0) or (temp <> header.insnum*2) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' FMK LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //fmk_file_loader
        end;
    end;

  If (header.patnum <> 0) then
    begin
      BlockReadF(f,paraptr_pat,header.patnum*4,temp);
      If (IOresult <> 0) or (temp <> header.patnum*4) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' FMK LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //fmk_file_loader
        end;
    end;

  init_songdata;
  load_flag := 0;

  If (header.init_spd <> 0) then speed := header.init_spd
  else speed := 1;

  If (header.base_spd <> 0) then tempo := header.base_spd
  else tempo := 50;

  songdata.tempo := tempo;
  songdata.speed := speed;
  songdata.songname := truncate_string(header.songname);
  songdata.composer := truncate_string(header.composer);
  songdata.common_flag := songdata.common_flag OR 1;
  songdata.common_flag := songdata.common_flag OR 2;
  songdata.common_flag := songdata.common_flag OR $80;

  For temp := 18 downto 1 do
    If NOT (header.trk_set[temp] AND 7 = 7) then BREAK;

  songdata.patt_len := 64;
  If adjust_tracks then songdata.nm_tracks := temp
  else If (songdata.nm_tracks < 18) then songdata.nm_tracks := 18;

  For temp2 := 1 to temp do
    If (header.trk_set[temp2] AND 7 = 6) then
      Case temp2 of
        1,2:   songdata.flag_4op := songdata.flag_4op OR 1;
        3,4:   songdata.flag_4op := songdata.flag_4op OR 2;
        5,6:   songdata.flag_4op := songdata.flag_4op OR 4;
        10,11: songdata.flag_4op := songdata.flag_4op OR 8;
        12,13: songdata.flag_4op := songdata.flag_4op OR $10;
        14,15: songdata.flag_4op := songdata.flag_4op OR $20;
      end;

  If (header.glob_var AND 1 = 1) then
    songdata.common_flag := songdata.common_flag OR $20;

  If (header.glob_var SHR 3 AND 1 = 1) then
    songdata.common_flag := songdata.common_flag OR 8;

  If (header.glob_var SHR 4 AND 1 = 1) then
    songdata.common_flag := songdata.common_flag OR $10;

  import_old_flags;
  If (header.glob_var AND 1 = 1) then
    begin
      Inc(songdata.lock_flags[1], _conv_fmk_pan[header.trk_pan[1] AND 3]);
      Inc(songdata.lock_flags[2], _conv_fmk_pan[header.trk_pan[1] SHR 2 AND 3]);
      Inc(songdata.lock_flags[3], _conv_fmk_pan[header.trk_pan[1] SHR 4 AND 3]);
      Inc(songdata.lock_flags[4], _conv_fmk_pan[header.trk_pan[1] SHR 6 AND 3]);
      Inc(songdata.lock_flags[5], _conv_fmk_pan[header.trk_pan[2] AND 3]);
      Inc(songdata.lock_flags[6], _conv_fmk_pan[header.trk_pan[2] SHR 2 AND 3]);
      Inc(songdata.lock_flags[7], _conv_fmk_pan[header.trk_pan[2] SHR 4 AND 3]);
      Inc(songdata.lock_flags[8], _conv_fmk_pan[header.trk_pan[2] SHR 6 AND 3]);
      Inc(songdata.lock_flags[9], _conv_fmk_pan[header.trk_pan[3] AND 3]);
      Inc(songdata.lock_flags[10],_conv_fmk_pan[header.trk_pan[3] SHR 2 AND 3]);
      Inc(songdata.lock_flags[11],_conv_fmk_pan[header.trk_pan[3] SHR 4 AND 3]);
      Inc(songdata.lock_flags[12],_conv_fmk_pan[header.trk_pan[3] SHR 6 AND 3]);
      Inc(songdata.lock_flags[13],_conv_fmk_pan[header.trk_pan[4] AND 3]);
      Inc(songdata.lock_flags[14],_conv_fmk_pan[header.trk_pan[4] SHR 2 AND 3]);
      Inc(songdata.lock_flags[15],_conv_fmk_pan[header.trk_pan[4] SHR 4 AND 3]);
      Inc(songdata.lock_flags[16],_conv_fmk_pan[header.trk_pan[4] SHR 6 AND 3]);
      Inc(songdata.lock_flags[17],_conv_fmk_pan[header.trk_pan[5] AND 3]);
      Inc(songdata.lock_flags[18],_conv_fmk_pan[header.trk_pan[5] SHR 2 AND 3]);
    end;

  For temp := 1 to max(header.ordnum,128) do
    Case order_list[temp-1] of
      255: songdata.pattern_order[temp-1] := $80;
      else songdata.pattern_order[temp-1] := order_list[temp-1];
    end;

  For temp := 1 to header.insnum do
    begin
      SeekF(f,paraptr_ins[temp]);
      If (IOresult <> 0) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' FMK LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //fmk_file_loader
        end;

      BlockReadF(f,insdata,SizeOf(insdata),temp2);
      If (IOresult <> 0) or (temp2 <> SizeOf(insdata)) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' FMK LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //fmk_file_loader
        end;

      If (truncate_string(insdata.iname) <> '') then
        songdata.instr_names[temp] :=
          Copy(songdata.instr_names[temp],1,9)+
          Copy(truncate_string(insdata.iname),1,32)
      else
        songdata.instr_names[temp] :=
          Copy(songdata.instr_names[temp],1,9)+
          truncate_string(insdata.dname);

      import_fin_instrument_alt (songdata.instr_data[temp], insdata.idata);
    end;

  For pat := 0 to PRED(header.patnum) do
    begin
      SeekF(f,paraptr_pat[pat]);
      If (IOresult <> 0) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' FMK LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //fmk_file_loader
        end;

      If (paraptr_pat[pat] = 0) then CONTINUE;
      BlockReadF(f,patlen,SizeOf(patlen),temp2);
      If (temp2 <> SizeOf(patlen)) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' FMK LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //fmk_file_loader
        end;

      If (patlen = 0) then CONTINUE;
      FillChar(buf1,SizeOf(buf1),0);
      BlockReadF(f,buf1,patlen,temp2);

      index := 0;
      row := 0;

      Repeat
        If (buf1[index] <> 0) then
          begin
            note := BYTE_NULL;
            ins  := 0;
            vol  := BYTE_NULL;
            cmd  := 0;
            info := 0;
            temp := buf1[index];
            Inc(index);

            chan := SUCC(temp AND 31);
            If (temp OR $20 = temp) then
              begin
                note := buf1[index];
                Inc(index);
                ins := buf1[index];
                Inc(index);
              end;

            If (temp OR $40 = temp) then
              begin
                vol := buf1[index];
                Inc(index);
              end;

            If (temp OR $80 = temp) then
              begin
                cmd := buf1[index];
                Inc(index);
                info := buf1[index];
                Inc(index);
              end;

            If (PRED(chan) in [1..18]) then
              import_fmk_event(pat,row,PRED(chan),note,ins,vol,cmd,info);
          end
        else
          begin
            Inc(row);
            Inc(index);
          end;
      until (row = 64);
    end;

  fix_fmk_commands(header.patnum);
  CloseF(f);
  songdata_title := NameOnly(songdata_source);
{$IFDEF ADT2PLAY}
  load_flag := 13;
{$ELSE}
  load_flag := 1;
{$ENDIF}

  _dbg_leave; //EXIT //fmk_file_loader
end;
