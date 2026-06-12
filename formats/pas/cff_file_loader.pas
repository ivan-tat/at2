// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure import_cff_event(patt,line,chan,byte0,byte1,byte2: Byte);

var
  chunk: tCHUNK;
  temp1,temp2,temp3,temp4: Byte;

begin
  FillChar(chunk,SizeOf(chunk),0);
  temp1 := byte2;
  temp2 := temp1 DIV 16;
  temp3 := temp1 MOD 16;

  Case CHAR(byte1) of
  { SET SPEED }
    'A': If (temp1 > 0) then
           begin
             chunk.effect_def := ef_SetSpeed;
             chunk.effect := temp1;
           end;

  { SET CARRIER WAVEFORM }
    'B': If (temp1 < 4) then
           begin
             chunk.effect_def := ef_SetWaveform;
             chunk.effect := temp1*16;
           end;

  { SET MODULATOR VOLUME }
    'C': begin
           chunk.effect_def := ef_SetModulatorVol;
           If (temp1 < 64) then chunk.effect := 63-temp1
           else chunk.effect := 0;
         end;

  { VOLUME SLIDE UP/DOWN }
    'D': begin
           chunk.effect_def := ef_VolSlide;
           chunk.effect := temp1;
         end;

  { SLIDE DOWN }
    'E': If (temp1 <> 0) then
           begin
             chunk.effect_def := ef_FSlideDown;
             chunk.effect := temp1;
           end;

  { SLIDE UP }
    'F': If (temp1 <> 0) then
           begin
             chunk.effect_def := ef_FSlideUp;
             chunk.effect := temp1;
           end;

  { SET CARRIER VOLUME }
    'G': begin
           chunk.effect_def := ef_SetCarrierVol;
           If (temp1 < 64) then chunk.effect := 63-temp1
           else chunk.effect := 0;
         end;

  { SET TEMPO }
    'H': If (temp1 > 0) then
           begin
             chunk.effect_def := ef_SetTempo;
             If NOT (temp1 > 21) then temp1 := 125;
             temp4 := 1412926 DIV LONGINT(temp1 SHR 1);
             chunk.effect := 1;
             While (PIT_FREQ_MAX DIV chunk.effect > temp4) and
                   (chunk.effect < 255) do
               Inc(chunk.effect);
           end;

  { SET INSTRUMENT }
    'I': If (temp1 < 47) then
           begin
             chunk.effect_def := ef_Extended;
             chunk.effect := ef_ex_ExtendedCmd2*16+ef_ex_cmd2_ResetVol;
             chunk.instr_def := temp1+1;
           end;

  { ARPEGGIO }
    'J': begin
           chunk.effect_def := ef_Arpeggio;
           chunk.effect := temp1;
         end;

  { JUMP TO ORDER }
    'K': If (temp1 < 128) then
           begin
             chunk.effect_def := ef_PositionJump;
             chunk.effect := temp1;
           end;

  { JUMP TO NEXT PATTERN IN ORDER }
    'L': chunk.effect_def := ef_PatternBreak;

  { SET TREMOLO HIGHER / SET VIBRATO DEEPER }
    'M': begin
           chunk.effect_def := ef_Extended;
           If (temp2 = 1) and (temp3 = 0) then chunk.effect := dec2hex(01);
           If (temp2 = 0) and (temp3 = 1) then chunk.effect := dec2hex(10);
           If (temp2 = 1) and (temp3 = 1) then chunk.effect := dec2hex(11);
         end;
  end;

  Case byte0 of
  { REGULAR NOTE }
    1..12*8+1: begin
                If NOT fix_c_note_bug then chunk.note := byte0
                else begin
                       chunk.note := byte0+1;
                       If (chunk.note > 12*8+1) then
                         chunk.note := 12*8+1;
                     end;
               end;
  { PAUSE }
    $6d: chunk.note := BYTE_NULL;
  end;

  put_chunk(patt,line,chan,chunk);
end;

procedure import_cff_patterns(var data; patterns: Byte);

type
  tPATDAT = array[0..$24] of
            array[0..$3f] of array[1..9] of
                             array[0..2] of Byte;

var
  voice: array[1..9] of Byte;
  arpgg: array[1..9] of Byte;
  chunk: tCHUNK;
  temp,temp2,temp3,temp4: Byte;
  order,patt: Byte;
  patt_break: Byte;
  patts: String;

function _empty_event(var data): Boolean;
begin
  _empty_event := (pBYTE(@data)[0] = 0) and
                  (pBYTE(@data)[1] = 0) and
                  (pBYTE(@data)[2] = 0);
end;

begin
  patts := '';
  FillChar(arpgg,SizeOf(arpgg),0);
  If NOT accurate_conv then
       For temp := 1 to 9 do voice[temp] := temp
  else For temp := 1 to 9 do voice[temp] := 0;

  For temp := 0 to $24 do
    For temp2 := 0 to $3f do
      For temp3 := 1 to 9 do
        If NOT _empty_event(tPATDAT(data)[temp][temp2][temp3]) then
          import_cff_event(temp,temp2,temp3,tPATDAT(data)[temp][temp2][temp3][0],
                                            tPATDAT(data)[temp][temp2][temp3][1],
                                            tPATDAT(data)[temp][temp2][temp3][2]);
  order := 0;
  patt := BYTE_NULL;

  Repeat
    If (songdata.pattern_order[order] > $24) then Inc(order)
    else
      begin
        patt := songdata.pattern_order[order];
        patt_break := BYTE_NULL;
        For temp2 := 0 to $3f do
          For temp3 := 1 to 9 do
             begin
               get_chunk(patt,temp2,temp3,chunk);
               temp4 := tPATDAT(data)[patt][temp2][temp3][2];

               Case CHAR(tPATDAT(data)[patt][temp2][temp3][1]) of
               { SET MODULATOR VOLUME }
                 'C': If (chunk.instr_def = 0) and NOT accurate_conv then
                        chunk.instr_def := voice[temp3]
                      else If (chunk.instr_def = 0) and
                              (voice[temp3] = 0) then chunk.instr_def := temp3;

               { SET CARRIER VOLUME }
                 'G': If (chunk.instr_def = 0) and NOT accurate_conv then
                        chunk.instr_def := voice[temp3]
                      else If (chunk.instr_def = 0) and
                              (voice[temp3] = 0) then chunk.instr_def := temp3;

               { SET INSTRUMENT }
                 'I': If (temp4 < 47) then
                        If (temp2 <> patt_break) then
                          begin
                            voice[temp3] := temp4+1;
                            If NOT accurate_conv then
                              chunk.instr_def := voice[temp3];
                          end;

               { ARPEGGIO }
                 'J': begin
                        chunk.effect_def := ef_Arpeggio;
                        If (temp4 <> 0) then
                          begin
                            chunk.effect := temp4;
                            arpgg[temp3] := temp4;
                          end
                        else chunk.effect := arpgg[temp3];
                      end;

               { JUMP TO ORDER }
                 'K': If (temp4 < 128) then
                        patt_break := temp2+1;

               { JUMP TO NEXT PATTERN IN ORDER }
                 'L': patt_break := temp2+1;
               end;

               Case tPATDAT(data)[patt][temp2][temp3][0] of
               { REGULAR NOTE }
                 1..12*8+1: begin
                             If accurate_conv then
                               If (voice[temp3] = 0) then
                                 begin
                                   voice[temp3] := temp3;
                                   chunk.instr_def := voice[temp3];
                                 end;

                              If NOT accurate_conv then
                                chunk.instr_def := voice[temp3];
                            end;
               end;

               If (Pos(CHR(songdata.pattern_order[order]),patts) = 0) then
                 put_chunk(patt,temp2,temp3,chunk);
             end;
        Inc(order);
        patts := patts+CHR(patt);
      end;
  until (patt >= patterns) or (order > $40);
end;

procedure cff_file_loader;

type
  tHEADER = Record
              ident: array[1..16] of Char;    { Identification }
              versn: Byte;                    { Format version }
              fsize: Word;                    { Filesize -32 }
              cflag: Byte;                    { Flag 1=compressed data }
              resrv: array[0..11] of Byte;    { Reserved }
            end;
type
  tINSDAT = Record
              iData: array[0..11] of Byte;    { Instrument data }
              iName: array[1..20] of Char;    { Instrument name }
            end;
type
  tHEADR2 = Record
              instr: array[0..46] of tINSDAT; { 47 instruments }
              nopat: Byte;                    { Number of patterns }
              ascii: array[1..31] of Char;    { ASCII blab }
              writr: array[1..20] of Char;    { Song writer }
              sname: array[1..20] of Char;    { Song name }
              order: array[0..64] of Byte;    { Pattern order }
            end;
const
  _PRE_ASCII_BLAB_SIZE = $5e1; // SizeOf(tHEADR2.instr)+SizeOf(tHEADR2.nopat)

const
  id = '<CUD-FM-File>'+#26+CHR($de)+CHR($e0);
  ascii_blab = 'CUD-FM-File - SEND A POSTCARD -';

var
  f: File;
  header: tHEADER;
  headr2: tHEADR2;
  temp,temp2: Longint;
  offs,out_size: Longint;

function LZTYR_decompress(var input,output): Longint;

type
  tSTRING = array[0..255] of Byte;

var
  input_idx: Longint;

  the_string,
  temp_string: tSTRING;

  old_code_length: Byte;
  repeat_length: Byte;
  repeat_counter: Longint;
  output_length: Longint;
  code_length: Byte;
  bits_buffer: Longint;
  bits_left: Word;
  old_code: Longint;
  new_code: Longint;
  idx: Word;

  _cff_heap_length: Word;
  _cff_dictionary_length: Word;
  _cff_dictionary: array[0..32767] of Pointer;

function get_code: Longint;

var
  code: Longint;

begin
  While (bits_left < code_length) do
    begin
      bits_buffer := bits_buffer OR (pBYTE(@input)[input_idx] SHL
                                     bits_left);
      Inc(input_idx);
      Inc(bits_left,8);
    end;

  code := bits_buffer AND ((1 SHL code_length)-1);
  bits_buffer := bits_buffer SHR code_length;
  Dec(bits_left,code_length);
  get_code := code;
end;

procedure translate_code(code: Longint; var str: tSTRING);

var
  translated_string: tSTRING;

begin
  If (code >= $104) then
    Move(_cff_dictionary[code-$104]^,translated_string,
         BYTE(_cff_dictionary[code-$104]^)+1)
  else begin
         translated_string[0] := 1;
         translated_string[1] := (code-4) AND $0ff;
        end;

  Move(translated_string,str,256);
end;

procedure startup;

var
  idx: Longint;

begin
  old_code := get_code;
  translate_code(old_code,the_string);

  If (the_string[0] > 0) then
    For idx := 0 to the_string[0]-1 do
      begin
        pBYTE(@output)[output_length] := the_string[idx+1];
        Inc(output_length);
      end;
end;

procedure cleanup;
begin
  code_length := 9;
  bits_buffer := 0;
  bits_left := 0;
  _cff_heap_length := 0;
  _cff_dictionary_length := 0;
end;

procedure expand__cff_dictionary(str: tSTRING);
begin
  If (str[0] >= $0f0) then
    EXIT; //expand__cff_dictionary
  Move(str,buf3[_cff_heap_length],str[0]+1);
  _cff_dictionary[_cff_dictionary_length] := Addr(buf3[_cff_heap_length]);
  Inc(_cff_dictionary_length);
  Inc(_cff_heap_length,str[0]+1);

  //EXIT //expand__cff_dictionary
end;

begin
  input_idx := 0;
  output_length := 0;
  cleanup;
  startup;

  Repeat
    new_code := get_code;

    // $00: end of data
    If (new_code = 0) then BREAK;

    // $01: end of block
    If (new_code = 1) then
      begin
        cleanup;
        startup;
        CONTINUE;
      end;

    // $02: expand code length
    If (new_code = 2) then
      begin
        Inc(code_length);
        CONTINUE;
      end;

    // $03: RLE
    If (new_code = 3) then
      begin
        old_code_length := code_length;
        code_length := 2;
        repeat_length := get_code+1;
        code_length := 4 SHL get_code;
        repeat_counter := get_code;

        For idx := 0 to PRED(repeat_counter*repeat_length) do
          begin
            pBYTE(@output)[output_length] :=
              pBYTE(@output)[output_length-repeat_length];
            Inc(output_length);
          end;

        code_length := old_code_length;
        startup;
        CONTINUE;
      end;

    If (new_code >= $104+_cff_dictionary_length) then
      begin
        Inc(the_string[0]);
        the_string[the_string[0]] := the_string[1];
      end
    else begin
           translate_code(new_code,temp_string);
           Inc(the_string[0]);
           the_string[the_string[0]] := temp_string[1];
         end;

    expand__cff_dictionary(the_string);
    translate_code(new_code,the_string);

    For idx := 0 to PRED(the_string[0]) do
      begin
        pBYTE(@output)[output_length] := the_string[idx+1];
        Inc(output_length);
      end;

    old_code := new_code;
  until FALSE;

  LZTYR_decompress := output_length;
end;

begin
  _dbg_enter ({$I %FILE%}, 'cff_file_loader');

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
             '~O~KAY$',' CFF LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //cff_file_loader
    end;

  BlockReadF(f,header,SizeOf(header),temp);
  If NOT ((temp = SizeOf(header)) and (header.ident = id)) or
     (FileSize(f) > SizeOf(buf1)) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' CFF LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //cff_file_loader
    end;

{$IFNDEF ADT2PLAY}
  If (play_status <> isStopped) then
    begin
      fade_out_playback(FALSE);
      stop_playing;
    end;
{$ELSE}
  load_flag := $7f;
{$ENDIF}
  If (header.cflag = 1) then
    begin
      FillChar(buf1,SizeOf(buf1),0);
      ResetF(f);
      BlockReadF(f,buf1,SizeOf(buf1),temp);
      If (IOresult <> 0) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - DiSK ERROR?$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' CFF LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //cff_file_loader
        end;

      CloseF(f);
      temp := LZTYR_decompress(buf1[$30],hash_buffer);
      out_size := temp;

      offs := SensitiveScan(hash_buffer,0,temp,ascii_blab);
      If (offs <> _PRE_ASCII_BLAB_SIZE) then
        begin
{$IFNDEF ADT2PLAY}
          Dialog('ERROR DECOMPRESSiNG MODULE DATA$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' CFF LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //cff_file_loader
        end;

      FillChar(buf1,SizeOf(buf1),0);
      Move(hash_buffer,headr2,SizeOf(headr2));
      Move(pBYTE(@hash_buffer)[SizeOf(headr2)],buf1,out_size-SizeOf(headr2));
    end
  else
    begin
      BlockReadF(f,headr2,SizeOf(headr2),temp);
      If NOT ((temp = SizeOf(headr2)) and (headr2.ascii = ascii_blab)) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' CFF LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //cff_file_loader
        end;

      FillChar(buf1,SizeOf(buf1),0);
      BlockReadF(f,buf1,SizeOf(buf1),temp);
      If (IOresult <> 0) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - DiSK ERROR?$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' CFF LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //cff_file_loader
        end;
      CloseF(f);
    end;

  init_songdata;
  load_flag := 0;

  songdata.patt_len := 64;
  If adjust_tracks then songdata.nm_tracks := 9
  else If (songdata.nm_tracks < 9) then songdata.nm_tracks := 9;

  tempo := 51;
  speed := 6;

  songdata.tempo := tempo;
  songdata.speed := speed;

  For temp2 := 0 to 64 do
    If (headr2.order[temp2] in [0..headr2.nopat]) then
      songdata.pattern_order[temp2] := headr2.order[temp2];

  For temp2 := 0 to 46 do
    begin
      import_hsc_instrument_alt (songdata.instr_data[temp2+1], headr2.instr[temp2].iData);
      songdata.instr_data[temp2+1].fine_tune := 0;
      songdata.instr_names[temp2+1] :=
        Copy(songdata.instr_names[temp2+1],1,9)+
        truncate_string(headr2.instr[temp2].iName);
    end;

  songdata.common_flag := songdata.common_flag OR 2;
  songdata.songname := CutStr(headr2.sname);
  songdata.composer := CutStr(headr2.writr);
  import_old_flags;

  import_cff_patterns(buf1,headr2.nopat);
  songdata_title := NameOnly(songdata_source);
{$IFDEF ADT2PLAY}
  load_flag := 7;
{$ELSE}
  load_flag := 1;
{$ENDIF}

  _dbg_leave; //EXIT //cff_file_loader
end;
