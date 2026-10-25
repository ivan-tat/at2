// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure dfm_file_loader;

const
  id = 'DFM'+#26;

var
  header: Record
            ident: array[1..4] of Char;
            versn: Word;
            sname: String[32];
            tempo: Byte;
            instn: array[1..32] of String[11];
            instd: array[1..32] of tFM_INST_DATA;
            order: array[1..128] of Byte;
            patts: Byte;
          end;

var
  f: File;
  temp,temp2,temp3: Longint;
  pattern,line,channel,byte1,byte2: Byte;

procedure import_dfm_event(patt,line,chan,byte1,byte2: Byte);

var
  chunk: tCHUNK;

begin
  FillChar(chunk,SizeOf(chunk),0);
  If (byte1 AND $0f in [1..12,15]) and ((byte1 SHR 4) AND 7 in [0..7]) then
    If (byte1 AND $0f <> 15) then
      chunk.note := SUCC(PRED(byte1 AND $0f)+((byte1 SHR 4) AND 7)*12)
    else chunk.note := BYTE_NULL;

  Case byte2 SHR 5 of
   { INSTRUMENT CHANGE }
    1: chunk.instr_def := SUCC(byte2 AND $1f);

   { SET INSTRUMENT VOLUME }
    2: begin
         chunk.effect_def := ef_SetInsVolume;
         chunk.effect := (byte2 AND $1f)*2;
       end;

   { TEMPO CHANGE }
    3: begin
         chunk.effect_def := ef_SetSpeed;
         chunk.effect := SUCC(byte2 AND $1f);
       end;

   { SLIDE UP }
    4: begin
         chunk.effect_def := ef_FSlideUpFine;
         chunk.effect := byte2 AND $1f;
       end;

   { SLIDE DOWN }
    5: begin
         chunk.effect_def := ef_FSlideDownFine;
         chunk.effect := byte2 AND $1f;
       end;

   { END OF PATTERN }
    7: chunk.effect_def := ef_PatternBreak;
  end;

  put_chunk(patt,line,chan,chunk);
end;

procedure process_dfm_patterns(patterns: Byte);

var
  chunk: tCHUNK;
  temp2,temp3: Byte;
  order,patt: Byte;
  patts: String;
  instr_cache: array[1..18] of Byte;

begin
  patts := '';
  FillChar(instr_cache,SizeOf(instr_cache),0);
  order := 0;
  patt := BYTE_NULL;

  Repeat
    If (songdata.pattern_order[order] >= $80) then Inc(order)
    else
      begin
        patt := songdata.pattern_order[order];
        For temp2 := 0 to $3f do
          For temp3 := 1 to 9 do
            begin
              get_chunk(patt,temp2,temp3,chunk);
              If (chunk.instr_def <> 0) then
                begin
                  chunk.effect_def := ef_Extended;
                  chunk.effect := ef_ex_ExtendedCmd2*16+ef_ex_cmd2_ResetVol;
                  instr_cache[temp3] := chunk.instr_def;
                  If NOT (chunk.note in [1..12*8+1]) and
                     NOT accurate_conv then
                    chunk.instr_def := 0;
                end
              else If (chunk.note in [1..12*8+1]) and
                      (chunk.instr_def = 0) and NOT accurate_conv then
                     chunk.instr_def := instr_cache[temp3];

              If (Pos(CHR(songdata.pattern_order[order]),patts) = 0) then
                put_chunk(patt,temp2,temp3,chunk);
            end;
        Inc(order);
        patts := patts+CHR(patt);
      end;
  until (patt >= patterns) or (order > $7f);
end;

begin
  _dbg_enter ({$I %FILE%}, 'dfm_file_loader');

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
             '~O~KAY$',' DFM LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //dfm_file_loader
    end;

  BlockReadF(f,header,SizeOf(header),temp);
  If NOT ((temp = SizeOf(header)) and (header.ident = id)) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' DFM LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //dfm_file_loader
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
  FillChar(buf1,SizeOf(buf1),0);
  BlockReadF(f,buf1,SizeOf(buf1),temp);
  If (IOresult <> 0) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('ERROR READiNG DATA - DiSK ERROR?$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' DFM LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //dfm_file_loader
    end;

  init_songdata;
  load_flag := 0;

  songdata.patt_len := 64;
  If adjust_tracks then songdata.nm_tracks := 9
  else If (songdata.nm_tracks < 9) then songdata.nm_tracks := 9;

  tempo := 135;
  speed := SUCC(header.tempo);

  songdata.songname := CutStr(header.sname);
  songdata.tempo := tempo;
  songdata.speed := speed;
  songdata.common_flag := songdata.common_flag OR 1;
  songdata.common_flag := songdata.common_flag OR 2;
  songdata.common_flag := songdata.common_flag OR 8;
  songdata.common_flag := songdata.common_flag OR $10;
  import_old_flags;

  For temp2 := 1 to 128 do
    If (header.order[temp2] in [0..$7f]) then
      songdata.pattern_order[temp2-1] := header.order[temp2]
    else If (header.order[temp2] = $80) then BREAK
         else songdata.pattern_order[temp2-1] := $80+temp2;

  For temp2 := 1 to 32 do
    begin
      songdata.instr_names[temp2] :=
        Copy(songdata.instr_names[temp2],1,9)+
        CutStr(header.instn[temp2]);
      While (BYTE(songdata.instr_names[temp2][
                    Length(songdata.instr_names[temp2])]) < 32) and
            (Length(songdata.instr_names[temp2]) <> 0) do
        Delete(songdata.instr_names[temp2],
               Length(songdata.instr_names[temp2]),1);
      import_standard_instrument_alt (songdata.instr_data[temp2], header.instd[temp2]);
    end;

  temp2 := 0;
  temp3 := 0;
  Repeat
    pattern := buf1[temp2];
    If (pattern > 127) then
      begin
        CloseF(f);
{$IFNDEF ADT2PLAY}
        Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
               'LOADiNG STOPPED$',
               '~O~KAY$',' DFM LOADER ',1);
{$ENDIF}
        _dbg_leave; EXIT; //dfm_file_loader
      end;

    Inc(temp2);
    Inc(temp3);

    For line := 0 to $3f do
      For channel := 1 to 9 do
        begin
          byte1 := buf1[temp2];
          If (temp2 >= temp) then
            begin
              CloseF(f);
{$IFNDEF ADT2PLAY}
              Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                     'LOADiNG STOPPED$',
                     '~O~KAY$',' DFM LOADER ',1);
{$ENDIF}
              _dbg_leave; EXIT; //dfm_file_loader
            end
          else Inc(temp2);

          If (byte1 OR $80 <> byte1) then byte2 := 0
          else begin
                 byte2 := buf1[temp2];
                 Inc(temp2);
               end;
          import_dfm_event(pattern,line,channel,byte1,byte2);
        end;
  until (temp2 >= temp);

  process_dfm_patterns(temp3);
  CloseF(f);

  songdata_title := NameOnly(songdata_source);
{$IFDEF ADT2PLAY}
  load_flag := 8;
{$ELSE}
  load_flag := 1;
{$ENDIF}

  _dbg_leave; //EXIT //dfm_file_loader
end;
