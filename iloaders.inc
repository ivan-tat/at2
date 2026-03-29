//  This file is part of Adlib Tracker II (AT2).
//
//  AT2 is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  AT2 is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with AT2.  If not, see <http://www.gnu.org/licenses/>.

// This file is included in "adt2ext3.pas" in implementation section

{
        function  import_sat_instrument_name(var data; inst: Byte): String;
        procedure sat_file_loader;
        function  _sal(op1,op2: Word): Byte;
        function  _sar(op1,op2: Word): Byte;
        procedure import_sa2_effect(effect,def1,def2: Byte;
                                    var out1,out2: Byte);
        procedure sa2_file_loader;
}

procedure import_old_flags; cdecl; external;
//a2m_file_loader
//a2t_file_loader
{$IFNDEF ADT2PLAY}
//a2p_file_loader
{$ENDIF}
//amd_file_loader
//cff_file_loader
//dfm_file_loader
//hsc_file_loader
//mtk_file_loader
//rad_file_loader

const
  temp_ef_Arpeggio = $0f0;
  temp_ef_rep      = $0f1;
  temp_ef_XFVSlide = $0f2;

//s3m_file_loader
//fmk_file_loader

function import_sat_instrument_name(var data; inst: Byte): String;

var
  temp1: Word;
  temp2: Byte;
  temp3: String;

begin
  temp1 := 0;
  temp2 := 0;
  temp3 := '';

  While (temp1 < 496) do
    begin
      If (pBYTE(@data)[temp1] = BYTE(#16)) then Inc(temp2);
      Inc(temp1);
      If (temp2 = inst+1) then
        begin
          While (pBYTE(@data)[temp1] in [$20..$0ff]) and
                (Length(temp3) < 22) do
            begin
              temp3 := temp3+CHR(pBYTE(@data)[temp1]);
              Inc(temp1);
            end;
          BREAK;
        end;
    end;

  import_sat_instrument_name := temp3;
end;

procedure import_sa2_effect(effect,def1,def2: Byte;
                            var out1,out2: Byte); forward;
procedure sat_file_loader;

type
  tHEADER = Record { version 1 }
              ident: array[1..4]   of Char; { ident_string }
              vernm: Byte;                  { version_number (1) }
              instt: array[0..$1e] of       { 31_instruments }
                     array[0..$0a] of Byte;
              instn: array[0..495] of Byte; { 31_instrument_names }
              order: array[0..254] of Byte; { pattern_order }
              nopat: Word;                  { number of patterns }
              snlen: Byte;                  { song_length }
              rspos: Byte;                  { restart_position }
              calls: Word;                  { calls_per_second }
            end;
type
  tHEADR2 = Record { version 6 }
              ident: array[1..4]   of Char; { ident_string }
              vernm: Byte;                  { version_number (1) }
              instt: array[0..$1e] of       { 31_instruments }
                     array[0..$0e] of Byte;
              instn: array[0..495] of Byte; { 31_instrument_names }
              order: array[0..$7f] of Byte; { pattern_order }
              nopat: Word;                  { number of patterns }
              snlen: Byte;                  { song_length }
              rspos: Byte;                  { restart_position }
              calls: Word;                  { calls_per_second }
              arpgd: array[1..512] of Byte; { arpeggio_data }
            end;
const
  id = 'SAdT';

var
  f: File;
  header: tHEADER;
  headr2: tHEADR2;
  SATver: Byte;
  temp,tmp2,tmp3,temp2,temp3,
  temp4,temp5: Longint;
  byte1,byte2,byte3,byte4,byte5,note_inc: Byte;

procedure import_sat_event(pattern,line,channel,
                           byte1,byte2,byte3,byte4,byte5: Byte);
var
  chunk: tCHUNK;

begin
  FillChar(chunk,SizeOf(chunk),0);
  If (byte2 in [1..31]) then chunk.instr_def := byte2;
  If (byte1 in [1..12*8+1]) then chunk.note := byte1+note_inc;

  import_sa2_effect(byte3,byte4,byte5,chunk.effect_def,chunk.effect);
  If (chunk.effect_def = ef_Extended) and
     (chunk.effect = ef_ex_ExtendedCmd2*16) and (chunk.note = 0) then
    begin
      chunk.note := BYTE_NULL;
      chunk.effect_def := 0;
      chunk.effect := 0;
    end;

  put_chunk(pattern,line,channel,chunk);
end;

var
  absolute: Longint;

function get_byte(var pos: Longint): Byte;
begin
  If (pos = SizeOf(buf1)-5) then
    begin
      If NOT (absolute > SizeOf(buf1)-5) then Move(buf3,buf1,SizeOf(buf3)-5)
      else Move(buf4,buf1,SizeOf(buf4)-5);
      pos := 0;
    end;
  get_byte := buf1[pos];
  Inc(pos);
  Inc(absolute);
end;

begin
  _dbg_enter ({$I %FILE%}, 'sat_file_loader');

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
             '~O~KAY$',' SAT LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //sat_file_loader
    end;

  BlockReadF(f,header,SizeOf(header),temp);
  If NOT ((temp = SizeOf(header)) and (header.ident = id)) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' SAT LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //sat_file_loader
    end;

  If NOT (header.vernm in [1,5,6]) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('UNKNOWN FiLE FORMAT VERSiON$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' SAT LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //sat_file_loader
    end;

{$IFDEF ADT2PLAY}
  load_flag := $7f;
{$ENDIF}
  SATver := header.vernm;
  If (SATver in [5,6]) then
    begin
      SeekF(f,0);
      If (IOresult <> 0) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' SAT LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //sat_file_loader
        end;

      BlockReadF(f,headr2,SizeOf(headr2),temp);
      If (temp <> SizeOf(headr2)) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' SAT LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //sat_file_loader
        end;
    end;

{$IFNDEF ADT2PLAY}
  If (play_status <> isStopped) then
    begin
      fade_out_playback(FALSE);
      stop_playing;
    end;

{$ENDIF}
  temp5 := (FileSize(f)-temp) DIV (64*9*5);
  FillChar(buf1,SizeOf(buf1),0);
  BlockReadF(f,buf1,SizeOf(buf1)-5,temp);
  If (IOresult <> 0) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('ERROR READiNG DATA - DiSK ERROR?$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' SAT LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //sat_file_loader
    end;

  tmp2 := WORD_NULL;
  If (temp = SizeOf(buf1)-5) then
    begin
      FillChar(buf3,SizeOf(buf3),0);
      BlockReadF(f,buf3,SizeOf(buf3)-5,tmp2);
      If (IOresult <> 0) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - DiSK ERROR?$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' SAT LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //sat_file_loader
        end;
    end;

  tmp3 := WORD_NULL;
  If (tmp2 = SizeOf(buf3)-5) then
    begin
      FillChar(buf4,SizeOf(buf4),0);
      BlockReadF(f,buf4,SizeOf(buf4)-5,tmp3);
      If (IOresult <> 0) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - DiSK ERROR?$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' SAT LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //sat_file_loader
        end;
    end;

  init_songdata;
  load_flag := 0;

  songdata.common_flag := songdata.common_flag OR 8;
  songdata.common_flag := songdata.common_flag OR $10;
  import_old_flags;

  songdata.patt_len := 64;
  If adjust_tracks then songdata.nm_tracks := 9
  else If (songdata.nm_tracks < 9) then songdata.nm_tracks := 9;

  For temp := 1 to 20 do
    songdata.lock_flags[temp] := songdata.lock_flags[temp] OR 4 OR 8;

  If (SATver = 1) then
    begin
      speed := 6;
      If (header.calls < 255) then tempo := header.calls
      else tempo := 255;

      songdata.tempo := tempo;
      songdata.speed := speed;

      For temp := 0 to max(header.snlen-1,127) do
        If (temp < 128) and (header.order[temp] in [0..63]) then
          songdata.pattern_order[temp] := header.order[temp];
      If (header.rspos < 128) and (SUCC(temp) < 128) then
        songdata.pattern_order[SUCC(temp)] := $80+header.rspos;

      temp5 := max(temp5,header.nopat);
      For temp := 0 to $1e do
        begin
          import_sat_instrument_alt (songdata.instr_data[temp+1], header.instt[temp]);
          songdata.instr_names[temp+1] :=
            Copy(songdata.instr_names[temp+1],1,9)+
            truncate_string(import_sat_instrument_name(header.instn,temp));
        end;
    end
  else
    begin
      speed := 6;
      If (headr2.calls < 255) then tempo := headr2.calls
      else tempo := 255;

      songdata.tempo := tempo;
      songdata.speed := speed;

      For temp := 0 to headr2.snlen-1 do
        If (temp < 128) and (headr2.order[temp] in [0..63]) then
          songdata.pattern_order[temp] := headr2.order[temp];
      If (headr2.rspos < 128) and (SUCC(temp) < 128) then
        songdata.pattern_order[SUCC(temp)] := $80+headr2.rspos;

      temp5 := max(temp5,headr2.nopat);
      For temp := 0 to $1e do
        begin
          import_sat_instrument_alt (songdata.instr_data[temp+1], headr2.instt[temp]);
          songdata.instr_names[temp+1] :=
            Copy(songdata.instr_names[temp+1],1,9)+
            truncate_string(import_sat_instrument_name(headr2.instn,temp));
        end;
    end;

  temp := 0;
  absolute := 0;

  Case SATver of
    1: note_inc := 24;
    5: note_inc := 12;
    6: note_inc := 0;
  end;

  For temp2 := 0 to temp5-1 do
    For temp3 := 0 to 63 do
      For temp4 := 1 to 9 do
        begin
          byte1 := get_byte(temp);
          byte2 := get_byte(temp);
          byte3 := get_byte(temp);
          byte4 := get_byte(temp);
          byte5 := get_byte(temp);
          import_sat_event(temp2,temp3,temp4,byte1,byte2,byte3,byte4,byte5);
        end;

  CloseF(f);
  songdata_title := NameOnly(songdata_source);
{$IFDEF ADT2PLAY}
  load_flag := 14;
{$ELSE}
  load_flag := 1;
{$ENDIF}

  _dbg_leave; //EXIT //sat_file_loader
end;

function _sal(op1,op2: Integer): Byte;
begin
  _sal := op1 SHL op2;
end;

function _sar(op1,op2: Integer): Byte;
begin
  _sar := op1 SHR op2;
end;

procedure import_sa2_effect(effect,def1,def2: Byte;
                            var out1,out2: Byte);
begin
  Case effect of
  { NORMAL PLAY OR ARPEGGIO }
    $00: begin
           out1 := ef_Arpeggio;
           out2 := def1*16+def2;
         end;

  { SLIDE UP }
    $01: begin
           out1 := ef_FSlideUp;
           out2 := def1*16+def2;
         end;

  { SLIDE DOWN }
    $02: begin
           out1 := ef_FSlideDown;
           out2 := def1*16+def2;
         end;

  { TONE PORTAMENTO }
    $03: begin
           out1 := ef_TonePortamento;
           out2 := def1*16+def2;
         end;

  { VIBRATO }
    $04: begin
           out1 := ef_Vibrato;
           out2 := def1*16+def2;
         end;

  { TONE PORTAMENTO + VOLUME SLIDE }
    $05: If (def1+def2 <> 0) then
           If (def1 in [1..15]) then
             begin
               out1 := ef_TPortamVolSlide;
               out2 := min(_sar(def1,2),1)*16;
             end
           else begin
                  out1 := ef_TPortamVolSlide;
                  out2 := min(_sar(def2,2),1);
                end
         else
           begin
             out1 := ef_TPortamVolSlide;
             out2 := def1*16+def2;
           end;

  { VIBRATO + VOLUME SLIDE }
    $06: If (def1+def2 <> 0) then
           If (def1 in [1..15]) then
             begin
               out1 := ef_VibratoVolSlide;
               out2 := min(_sar(def1,2),1)*16;
             end
           else begin
                  out1 := ef_VibratoVolSlide;
                  out2 := min(_sar(def2,2),1);
                end
         else
           begin
             out1 := ef_VibratoVolSlide;
             out2 := def1*16+def2;
           end;

  { RELEASE SUSTAINING SOUND }
    $08: begin
           out1 := ef_Extended;
           out2 := ef_ex_ExtendedCmd2*16+0;
         end;

  { VOLUME SLIDE }
    $0a: If (def1+def2 <> 0) then
           If (def1 in [1..15]) then
             begin
               out1 := ef_VolSlide;
               out2 := min(_sar(def1,2),1)*16;
             end
           else begin
                  out1 := ef_VolSlide;
                  out2 := min(_sar(def2,2),1);
                end
         else
           begin
             out1 := ef_VolSlide;
             out2 := def1*16+def2;
           end;

  { POSITION JUMP }
    $0b: If (def1*16+def2 < 128) then
           begin
             out1 := ef_PositionJump;
             out2 := def1*16+def2;
           end;

  { SET VOLUME }
    $0c: begin
           out1 := ef_SetInsVolume;
           out2 := def1*16+def2;
           If (out2 > 63) then out2 := 63;
         end;

  { PATTERN BREAK }
    $0d: If (def1*16+def2 < 64) then
           begin
             out1 := ef_PatternBreak;
             out2 := def1*16+def2;
           end;

  { SET SPEED }
    $0f: If (def1*16+def2 < $20) then
           begin
             out1 := ef_SetSpeed;
             out2 := def1*16+def2;
           end
         else If (def1 < 16) and (def2 < 16) then
                begin
                  out1 := ef_SetTempo;
                  out2 := Round((def1*16+def2)/2.5);
                end;
    else begin
           out1 := 0;
           out2 := 0;
         end;
  end;
end;

procedure sa2_file_loader;

type
  tHEADER = Record
              ident: array[1..4]   of Char; { These bytes mark a song }
              vernm: Byte;                  { Version number (9) }
              instt: array[0..$1e] of       { 31 instruments }
                     array[0..$0e] of Byte;
              instn: array[0..495] of Byte; { 31_instrument_names }
              order: array[0..$7f] of Byte; { Pattern order }
              nopat: Word;                  { Number of patterns }
              snlen: Byte;                  { Length of song }
              rspos: Byte;                  { Restart position }
              snbpm: Word;                  { BPM }
              arpgd: array[1..512] of Byte; { Arpeggio data (list+commands) }
              ordr2: array[0..63]  of       { Track order }
                     array[1..9]   of Byte;
              chans: Word;                  { Active channels }
            end;
const
  id = 'SAdT';

var
  f: File;
  header: tHEADER;
  temp,temp2,temp3,temp4,temp5: Longint;

procedure import_sa2_event(pattern,line,channel,
                           byte1,byte2,byte3: Byte);
var
  chunk: tCHUNK;
  temp: Byte;

begin
  FillChar(chunk,SizeOf(chunk),0);
  temp := (byte1 AND 1) SHL 4 +(byte2 SHR 4);
  If (temp in [1..31]) then chunk.instr_def := temp;
  If (byte1 SHR 1 in [1..12*8+1]) then chunk.note := (byte1 SHR 1);

  import_sa2_effect(byte2 AND $0f,byte3 SHR 4,byte3 AND $0f,
                    chunk.effect_def,chunk.effect);
  If (chunk.effect_def = ef_Extended) and
     (chunk.effect = ef_ex_ExtendedCmd2*16) and (chunk.note = 0) then
    begin
      chunk.note := BYTE_NULL;
      chunk.effect_def := 0;
      chunk.effect := 0;
    end;

  put_chunk(pattern,line,channel,chunk);
end;

begin { sa2_file_loader }
  _dbg_enter ({$I %FILE%}, 'sa2_file_loader');

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
             '~O~KAY$',' SA2 LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //sa2_file_loader
    end;

  BlockReadF(f,header,SizeOf(header),temp);
  If NOT ((temp = SizeOf(header)) and (header.ident = id)) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' SA2 LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //sa2_file_loader
    end;

  If NOT (header.vernm in [8,9]) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('UNKNOWN FiLE FORMAT VERSiON$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' SA2 LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //sa2_file_loader
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
  If (header.vernm = 8) then
    begin
      SeekF(f,FilePos(f)-2);
      If (IOresult <> 0) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - DiSK ERROR?$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' SA2 LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //sa2_file_loader
        end;
    end;

  FillChar(buf1,SizeOf(buf1),0);
  BlockReadF(f,buf1,SizeOf(buf1)-3,temp);
  If (IOresult <> 0) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('ERROR READiNG DATA - DiSK ERROR?$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' SA2 LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //sa2_file_loader
    end;

  init_songdata;
  load_flag := 0;

  songdata.common_flag := songdata.common_flag OR 8;
  songdata.common_flag := songdata.common_flag OR $10;
  import_old_flags;

  songdata.patt_len := 64;
  If adjust_tracks then songdata.nm_tracks := 9
  else If (songdata.nm_tracks < 9) then songdata.nm_tracks := 9;

  For temp := 1 to 20 do
    songdata.lock_flags[temp] := songdata.lock_flags[temp] OR 4 OR 8;

  speed := 6;
  If (Round(header.snbpm/2.5) < 255) then tempo := Round(header.snbpm/2.5)
  else tempo := 255;

  songdata.tempo := tempo;
  songdata.speed := speed;

  temp2 := 0;
  temp3 := 0;
  temp4 := 1;

  Repeat
    While (header.ordr2[temp2][temp4] = 0) and
          (temp2 <= header.nopat-1) do
      begin
        Inc(temp4);
        If (temp4 > 9) then begin temp4 := 1; Inc(temp2); end;
      end;

    If (temp2 <= header.nopat-1) then
      begin
        temp5 := 64*3*(header.ordr2[temp2][temp4]-1)+temp3*3;
        import_sa2_event(temp2,temp3,temp4,buf1[temp5],
                                           buf1[temp5+1],
                                           buf1[temp5+2]);
        Inc(temp3);
        If (temp3 > $3f) then
          begin
            temp3 := 0;
            If (temp4 < 9) then Inc(temp4)
            else begin temp4 := 1; Inc(temp2); end;
          end;
      end;
  until (temp2 > header.nopat-1);

  For temp := 0 to header.snlen-1 do
    If (temp < 128) and (header.order[temp] in [0..63]) then
      songdata.pattern_order[temp] := header.order[temp];
  If (header.rspos < 128) and (SUCC(temp) < 128) then
    songdata.pattern_order[SUCC(temp)] := $80+header.rspos;

  For temp := 0 to $1e do
    begin
      import_sat_instrument_alt (songdata.instr_data[temp+1], header.instt[temp]);
      songdata.instr_names[temp+1] := Copy(songdata.instr_names[temp+1],1,9)+
        truncate_string(import_sat_instrument_name(header.instn,temp));
    end;

  CloseF(f);
  songdata_title := NameOnly(songdata_source);
{$IFDEF ADT2PLAY}
  load_flag := 15;
{$ELSE}
  load_flag := 1;
{$ENDIF}

  _dbg_leave; //EXIT //sa2_file_loader
end;
