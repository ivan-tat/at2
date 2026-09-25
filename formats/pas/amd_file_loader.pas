// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure amd_file_loader;

type
  tPATDAT = array[0..$24] of
            array[0..$3f] of array[1..9] of
                             array[0..2] of Byte;
type
  tINSDAT = Record
              iName: array[1..23] of Char;     { Instrument name }
              iData: array[0..10] of Byte;     { Instrument data }
            end;
type
  tHEADER = Record
              sname: array[1..24]  of Char;    { Name of song [ASCIIZ] }
              aname: array[1..24]  of Char;    { Name of author [ASCIIZ] }
              instr: array[0..25]  of tINSDAT; { 26 instruments }
              snlen: Byte;                     { Song length }
              nopat: Byte;                     { Number of patterns -1 }
              order: array[0..$7f] of Byte;    { Pattern table }
              ident: array[1..9]   of Char;    { ID }
              versn: Byte;                     { Version 10h=normal module }
                                               {         11h=packed module }
            end;
const
  id_amd = '<o'#239'QU'#238'RoR';
  id_xms = 'MaDoKaN96';

var
  f: File;
  header: tHEADER;
  temp,tmp2,temp2,temp3,temp4: Longint;
  byte1,byte2,byte3: Byte;

procedure import_amd_instrument(inst: Byte; var data);
begin
  With songdata.instr_data[inst] do
    begin
      fm_data.AM_VIB_EG_modulator := pBYTE(@data)[0];
      fm_data.KSL_VOLUM_modulator := pBYTE(@data)[1];
      fm_data.ATTCK_DEC_modulator := pBYTE(@data)[2];
      fm_data.SUSTN_REL_modulator := pBYTE(@data)[3];
      fm_data.WAVEFORM_modulator  := pBYTE(@data)[4]  AND 3;
      fm_data.AM_VIB_EG_carrier   := pBYTE(@data)[5];
      fm_data.KSL_VOLUM_carrier   := pBYTE(@data)[6];
      fm_data.ATTCK_DEC_carrier   := pBYTE(@data)[7];
      fm_data.SUSTN_REL_carrier   := pBYTE(@data)[8];
      fm_data.WAVEFORM_carrier    := pBYTE(@data)[9]  AND 3;
      fm_data.FEEDBACK_FM         := pBYTE(@data)[10] AND $0f;
    end;

  songdata.instr_data[inst].panning := 0;
  songdata.instr_data[inst].fine_tune := 0;
end;

procedure import_amd_event(pattern,line,channel,byte1,byte2,byte3: Byte);

var
  chunk: tCHUNK;
  param: Byte;

begin
  FillChar(chunk,SizeOf(chunk),0);
  If ((byte2 SHR 4)+(byte1 AND 1) SHL 4 <> 0) then
    chunk.instr_def := (byte2 SHR 4)+(byte1 AND 1) SHL 4;

  If (byte1 SHR 4 in [1..12]) and ((byte1 SHR 1) AND 7 in [0..7]) then
    chunk.note := 12*((byte1 SHR 1) AND 7)+(byte1 SHR 4);

  param := byte3 AND $7f;
  Case byte2 AND $0f of
  { ARPEGGIO }
    $00: begin
           chunk.effect_def := ef_Arpeggio;
           chunk.effect := dec2hex(param);
         end;

  { SLIDE FREQUENCY UP }
    $01: begin
           chunk.effect_def := ef_FSlideUp;
           chunk.effect := param;
         end;

  { SLIDE FREQUENCY DOWN }
    $02: begin
           chunk.effect_def := ef_FSlideDown;
           chunk.effect := param;
         end;

  { SET CARRIER/MODULATOR INTENSITY }
    $03: If (param DIV 10 in [1..9]) then
           begin
             chunk.effect_def := ef_SetCarrierVol;
             chunk.effect := (param DIV 10)*7;
           end
         else If (param MOD 10 in [1..9]) then
                begin
                  chunk.effect_def := ef_SetModulatorVol;
                  chunk.effect := (param MOD 10)*7;
                end;

  { SET THE VOLUME }
    $04: begin
           chunk.effect_def := ef_SetInsVolume;
           If (param < 64) then chunk.effect := param
           else chunk.effect := 63;
         end;

  { JUMP INTO PATTERN }
    $05: begin
           chunk.effect_def := ef_PositionJump;
           If (param < 100) then chunk.effect := param
           else chunk.effect := 99;
         end;

  { PATTERNBREAK }
    $06: begin
           chunk.effect_def := ef_PatternBreak;
           If (param < 64) then chunk.effect := param
           else chunk.effect := 63;
         end;

  { SET SONGSPEED }
    $07: If (param < 99) then
           If (param in [1..31]) then
             begin
               chunk.effect_def := ef_SetSpeed;
               chunk.effect := param;
             end
           else begin
                  chunk.effect_def := ef_SetTempo;
                  If (param = 0) then chunk.effect := 18
                  else chunk.effect := param;
                end;

  { TONEPORTAMENTO }
    $08: begin
           chunk.effect_def := ef_TonePortamento;
           chunk.effect := param;
         end;

  { EXTENDED COMMAND }
    $09: If (param < 60) then
           Case param DIV 10 of
           { DEFINE CELL-TREMOLO }
             0: If (param MOD 10 < 2) then
                  begin
                    chunk.effect_def := ef_Extended;
                    chunk.effect := dec2hex(param);
                  end;

           { DEFINE CELL-VIBRATO }
             1: If (param MOD 10 < 2) then
                  begin
                    chunk.effect_def := ef_Extended;
                    chunk.effect := $10+dec2hex(param);
                  end;

           { INCREASE VOLUME FAST }
             2: begin
                  chunk.effect_def := ef_VolSlide;
                  chunk.effect := (param MOD 10)*16;
                end;

           { DECREASE VOLUME FAST }
             3: begin
                  chunk.effect_def := ef_VolSlide;
                  chunk.effect := param MOD 10;
                end;

           { INCREASE VOLUME FINE }
             4: begin
                  chunk.effect_def := ef_Extended2;
                  chunk.effect := ef_ex2_VolSlideUpXF*16+(param MOD 10);
                end;

           { DECREASE VOLUME FINE }
             5: begin
                  chunk.effect_def := ef_Extended2;
                  chunk.effect := ef_ex2_VolSlideDnXF*16+(param MOD 10);
                end;
           end;
  end;

// specific corrections for Amusic event
  If (chunk.note = 0) then chunk.instr_def := 0;
  put_chunk(pattern,line,channel,chunk);
end;

procedure import_amd_packed_patterns(var data; patterns: Byte);

var
  temp,temp2,temp3,temp4,temp5: Word;
  count: Byte;

var
  tracks: Word;
  track_order: array[0..$3f] of array[1..9] of Word;
  track: array[0..$3f] of tCHUNK;

begin
  temp := (patterns+1)*9*SizeOf(WORD);
  Move(data,track_order,temp);

  tracks := pBYTE(@data)[temp]+(pBYTE(@data)[temp+1]) SHL 8;
  Inc(temp,2);

  temp3 := 0;
  temp4 := 0;
  count := 0;

  Repeat
    If (count = 0) then
      begin
        If (temp3 = 0) then
          begin
            temp2 := pBYTE(@data)[temp]+(pBYTE(@data)[temp+1]) SHL 8;
            Inc(temp,2);
          end;

        If (pBYTE(@data)[temp] OR $80 <> pBYTE(@data)[temp]) then
          begin
            If (temp2 DIV 9 <= $3f) and (temp2 MOD 9 < 9) then
              import_amd_event(temp2 DIV 9,temp3,temp2 MOD 9 +1,
                               pBYTE(@data)[temp+2],
                               pBYTE(@data)[temp+1],
                               pBYTE(@data)[temp+0]);
            Inc(temp,3);
          end
        else
          begin
            count := (pBYTE(@data)[temp] AND $7f)-1;
            Inc(temp);
          end;
      end
    else Dec(count);

    Inc(temp3);
    If (temp3 > $3f) then
      begin
        temp3 := 0;
        count := 0;
        Inc(temp4);
      end;
  until NOT (temp4 < tracks);

  For temp := 0 to patterns do
    For temp2 := 1 to 9 do
      begin
        temp3 := track_order[temp][temp2];
        temp4 := temp3 DIV 9;

        If (temp3 < 64*9) then
          begin
            For temp5 := 0 to $3f do
              get_chunk(temp4,temp5,temp3 MOD 9 +1,track[temp5]);
            For temp5 := 0 to $3f do
              put_chunk(temp,temp5,temp2,track[temp5]);
          end;
      end;
end;

function get_byte(var pos: Longint): Byte;
begin
  If (pos = SizeOf(buf1)) then
    begin
      Move(buf3,buf1,SizeOf(buf3));
      pos := 0;
    end;
  get_byte := buf1[pos];
  Inc(pos);
end;

begin
  _dbg_enter ({$I %FILE%}, 'amd_file_loader');

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
             '~O~KAY$',' '+iCASE(ExtOnly(songdata_source))+' LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //amd_file_loader
    end;

  BlockReadF(f,header,SizeOf(header),temp);
  If NOT ((temp = SizeOf(header)) and
         ((header.ident = id_amd) or (header.ident = id_xms))) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' '+iCASE(ExtOnly(songdata_source))+' LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //amd_file_loader
    end;

{$IFDEF ADT2PLAY}
  load_flag := $7f;
{$ENDIF}
  If NOT (header.versn in [$10,$11]) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('UNKNOWN FiLE FORMAT VERSiON$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' '+iCASE(ExtOnly(songdata_source))+' LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //amd_file_loader
    end;

{$IFNDEF ADT2PLAY}
  If (play_status <> isStopped) then
    begin
      fade_out_playback(FALSE);
      stop_playing;
    end;
{$ENDIF}

  FillChar(buf1,SizeOf(buf1),0);
  BlockReadF(f,buf1,SizeOf(buf1),temp);
  If (IOresult <> 0) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' '+iCASE(ExtOnly(songdata_source))+' LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //amd_file_loader
    end;

  tmp2 := WORD_NULL;
  If (temp = SizeOf(buf1)) then
    begin
      FillChar(buf3,SizeOf(buf3),0);
      BlockReadF(f,buf3,SizeOf(buf3),tmp2);
      If (IOresult <> 0) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' '+iCASE(ExtOnly(songdata_source))+' LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //amd_file_loader
        end;
    end;

  init_songdata;
  load_flag := 0;

  songdata.patt_len := 64;
  If adjust_tracks then songdata.nm_tracks := 9
  else If (songdata.nm_tracks < 9) then songdata.nm_tracks := 9;

  tempo := 50;
  speed := 6;

  songdata.tempo := tempo;
  songdata.speed := speed;

  For temp2 := 0 to header.snlen-1 do
    If (temp2 < 128) and (header.order[temp2] in [0..header.nopat]) then
      songdata.pattern_order[temp2] := header.order[temp2];

  For temp2 := 0 to 25 do
    begin
      import_amd_instrument(temp2+1,header.instr[temp2].iData);
      songdata.instr_names[temp2+1] :=
        Copy(songdata.instr_names[temp2+1],1,9)+
        truncate_string(header.instr[temp2].iName);
    end;

  temp := 0;
  If (header.versn = $10) then
    For temp2 := 0 to header.nopat do
      For temp3 := 0 to $3f do
        For temp4 := 1 to 9 do
          begin
            byte3 := get_byte(temp);
            byte2 := get_byte(temp);
            byte1 := get_byte(temp);
            import_amd_event(temp2,temp3,temp4,byte1,byte2,byte3);
          end
  else
    import_amd_packed_patterns(buf1,header.nopat);

  songdata.common_flag := songdata.common_flag OR $80;
  songdata.songname := CutStr(asciiz_string(header.sname));
  songdata.composer := CutStr(asciiz_string(header.aname));
  import_old_flags;

  CloseF(f);
  songdata_title := NameOnly(songdata_source);
{$IFDEF ADT2PLAY}
  If (header.ident = id_amd) then load_flag := 5
  else load_flag := 6;
{$ELSE}
  load_flag := 1;
{$ENDIF}

  _dbg_leave; //EXIT //amd_file_loader
end;
