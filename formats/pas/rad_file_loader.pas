// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure rad_file_loader;

const
  id = 'RAD by REALiTY!!';

var
  header: Record
            ident: array[1..16] of Char; { Use this to recognize a RAD tune }
            rmver: Byte;                 { Version of RAD file (10h) }
            xbyte: Byte;       { bit7      Set if a description follows }
          end;                 { bit6      Set if it's a "slow-timer" tune }
                               { bit[4..0] The initial speed of the tune }
var
  f: File;
  dscbuf: array[0..PRED(80*22)] of Char;
  pattoffs: array[0..$1f] of Word;
  temp,temp2,temp3,temp4,temp5,offs0: Longint;

procedure import_rad_event(pattern,line,channel,byte1,byte2,byte3: Byte);

var
  chunk: tCHUNK;

begin
  FillChar(chunk,SizeOf(chunk),0);
  If ((byte2 SHR 4)+(byte1 SHR 7) SHL 4 <> 0) then
    chunk.instr_def := (byte2 SHR 4)+(byte1 SHR 7) SHL 4;

  If (byte1 AND $0f in [1..12]) then chunk.note := 12*((byte1 SHR 4) AND 7)+(byte1 AND $0f)+1
  else If (byte1 AND $0f = $0f) then chunk.note := BYTE_NULL;

  Case byte2 AND $0f of
  { PORTAMENTO (FREQUENCY SLIDE) UP }
    $01: begin
           chunk.effect_def := ef_FSlideUp;
           chunk.effect := byte3;
         end;

  { PORTAMENTO (FREQUENCY SLIDE) DOWN }
    $02: begin
           chunk.effect_def := ef_FSlideDown;
           chunk.effect := byte3;
         end;

  { PORTAMENTO TO NOTE }
    $03: begin
           chunk.effect_def := ef_TonePortamento;
           chunk.effect := byte3;
         end;

  { PORTAMENTO TO NOTE WITH VOLUME SLIDE }
    $05: If (byte3 in [1..49]) then
           begin
             chunk.effect_def := ef_TPortamVolSlide;
             chunk.effect := max(byte3,15);

             If (byte3 > 15) then
               begin
                 chunk.effect_def2 := ef_TPortamVolSlide;
                 chunk.effect2 := max(byte3-15,15);
               end;
           end
         else If (byte3 in [51..99]) then
                begin
                  chunk.effect_def := ef_TPortamVolSlide;
                  chunk.effect := max(byte3-50,15)*16;

                  If (byte3-50 > 15) then
                    begin
                      chunk.effect_def2 := ef_TPortamVolSlide;
                      chunk.effect2 := max(byte3-50-15,15);
                    end;
                end;

  { VOLUME SLIDE }
    $0a: If (byte3 in [1..49]) then
           begin
             chunk.effect_def := ef_VolSlide;
             chunk.effect := max(byte3,15);

             If (byte3 > 15) then
               begin
                 chunk.effect_def2 := ef_VolSlide;
                 chunk.effect2 := max(byte3-15,15);
               end;
           end
         else If (byte3 in [51..99]) then
                begin
                  chunk.effect_def := ef_VolSlide;
                  chunk.effect := max(byte3-50,15)*16;

                  If (byte3-50 > 15) then
                    begin
                      chunk.effect_def2 := ef_VolSlide;
                      chunk.effect2 := max(byte3-50-15,15);
                    end;
                end;

  { SET VOLUME }
    $0c: begin
           chunk.effect_def := ef_SetInsVolume;
           If (byte3 < 64) then chunk.effect := byte3
           else chunk.effect := 63;
         end;

  { JUMP TO NEXT PATTERN IN ORDER LIST }
    $0d: begin
           chunk.effect_def := ef_PatternBreak;
           If (byte3 < 64) then chunk.effect := byte3
           else chunk.effect := 63;
         end;

  { SET SPEED }
    $0f: begin
           chunk.effect_def := ef_SetSpeed;
           chunk.effect := byte3;
         end;
  end;

// specific corrections for RAd-Tracker event
  If (chunk.effect_def in [ef_TonePortamento,
                           ef_TPortamVolSlide]) and
     (chunk.note = BYTE_NULL) then chunk.note := 0;
  If (chunk.effect_def in [ef_TonePortamento,
                           ef_TPortamVolSlide]) then chunk.instr_def := 0;
  If (chunk.note = 0) then chunk.instr_def := 0;
  put_chunk(pattern,line,channel+1,chunk);
end;

// *******************************
// ADT2plug RAD description loader
// *******************************

{$IFDEF PLUGIN}
procedure load_rad_description(length: Word);

var
  temp: Word;
  row,temp2: Byte;
  xstart,ystart: Byte;
  desc: String;

procedure insertdesc;
begin
  radinstr_used := true;
  While (SYSTEM.Length(desc) > maxmemolinelen) do
    begin
      radinstr_names[row] := Copy(desc,1,maxmemolinelen);
      desc := Copy(desc,maxmemolinelen+1,system.length(desc)-maxmemolinelen);
      Inc(row);
    end;
  radinstr_names[row] := desc;
end;

begin
  temp := 0;
  row  := 1;
  desc := '';
  While (dscbuf[temp] <> #0) and (temp < length) do
    begin
      Case dscbuf[temp] of
        #1: If (row < 255) then
              begin
                insertdesc;
                Inc(row);
                desc := '';
              end;
        #2..#31: For temp2 := 1 to BYTE(dscbuf[temp]) do
                   desc := desc+' ';
        #32..#255: desc := desc+dscbuf[temp];
      end;
      Inc(temp);
    end;
  insertdesc;
end;
{$ENDIF}

{$IFNDEF ADT2PLAY}
procedure list_rad_description(length: Word);

var
  desc: array[1..22] of String[80];
  temp,fkey: Word;
  row,temp2: Byte;
  xstart,ystart: Byte;

begin
  FillChar(desc,SizeOf(desc),0);
  temp := 0;
  row  := 1;

  While (dscbuf[temp] <> #0) and (temp < length) do
    begin
      Case dscbuf[temp] of
        #1: If (row < 22) then Inc(row);
        #2..#31: For temp2 := 1 to BYTE(dscbuf[temp]) do
                   desc[row] := desc[row]+' ';
        #32..#255: desc[row] := desc[row]+dscbuf[temp];
      end;
      Inc(temp);
    end;

  temp2 := 0;
  For temp := 1 to 22 do
    If (truncate_string(desc[temp]) <> '') then Inc(temp2);
  If (temp2 = 0) then
    EXIT; //list_rad_description

  ScreenMemCopy(screen_ptr,ptr_screen_backup);
  centered_frame_vdest := screen_ptr;
  centered_frame(xstart,ystart,81,24,' RAD DESCRiPTiON ',
                 dialog_background+dialog_border,
                 dialog_background+dialog_title,
                 frame_double);

  For temp := 1 to 22 do
    ShowStr(screen_ptr,xstart+1,ystart+temp,FilterStr2(desc[temp],_valid_characters,'_'),
            dialog_background+dialog_context_dis);

  ShowCStr(screen_ptr,xstart+36,ystart+23,' ~C~ONTiNUE ',
           dialog_sel_itm_bck+dialog_sel_itm,
           dialog_sel_itm_bck+dialog_sel_short);

  Repeat fkey := GetKey;
  until (fkey = kESC) or (fkey = kENTER) or (HI(fkey) = $2e);

  move_to_screen_data := ptr_screen_backup;
  move_to_screen_area[1] := xstart;
  move_to_screen_area[2] := ystart;
  move_to_screen_area[3] := xstart+81+2;
  move_to_screen_area[4] := ystart+24+1;
  move2screen;
  no_status_refresh := TRUE;

  //EXIT //list_rad_description
end;
{$ENDIF}

begin
  _dbg_enter ({$I %FILE%}, 'rad_file_loader');

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
             '~O~KAY$',' RAD LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //rad_file_loader
    end;

  BlockReadF(f,header,SizeOf(header),temp);
  If NOT ((temp = SizeOf(header)) and (header.ident = id)) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' RAD LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //rad_file_loader
    end;

  If (header.rmver <> $10) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('UNKNOWN FiLE FORMAT VERSiON$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' RAD LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //rad_file_loader
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
             '~O~KAY$',' RAD LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //rad_file_loader
    end;

  temp2 := 0;
  offs0 := SizeOf(header);

  If (header.xbyte OR $80 = header.xbyte) then
    begin
      While (temp2 < temp) and (buf1[temp2] <> 0) do Inc(temp2);
      If (temp2 >= temp) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' RAD LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //rad_file_loader
        end;

      Inc(offs0,temp2+1);
      Dec(temp,temp2+1);
      Move(buf1,dscbuf,temp2+1);
      Move(buf1[temp2+1],buf1,temp);
{$IFDEF PLUGIN}
      load_rad_description(temp2+1);
{$ENDIF}
    end;

{$IFNDEF ADT2PLAY}
  If mod_description and (temp2 <> 0) and NOT quick_cmd and
     NOT shift_pressed then
    list_rad_description(temp2+1);
{$ENDIF}

  init_songdata;
  load_flag := 0;

  songdata.patt_len := 64;
  If adjust_tracks then songdata.nm_tracks := 9
  else If (songdata.nm_tracks < 9) then songdata.nm_tracks := 9;

  If (header.xbyte OR $40 = header.xbyte) then tempo := 18
  else tempo := 50;

  If (header.xbyte AND $1f in [1..31]) then speed := header.xbyte AND $1f
  else speed := 2;

  songdata.tempo := tempo;
  songdata.speed := speed;

  temp2 := 0;
  Repeat
    temp3 := buf1[temp2];
    Inc(temp2);
    If (temp3 <> 0) and (temp2+11 < temp) then
      begin
        import_hsc_instrument_alt (songdata.instr_data[temp3], buf1[temp2]);
        songdata.instr_data[temp3].fine_tune := 0;
        Inc(temp2,11);
      end;
  until (temp3 = 0) or (temp3 >= temp);

  Inc(offs0,temp2);
  Dec(temp,temp2);
  Move(buf1[temp2],buf1,temp);

  Inc(offs0,buf1[0]+1);
  If (buf1[0] <> 0) then
    Move(buf1[1],songdata.pattern_order,buf1[0]);

  Inc(offs0,32*SizeOf(WORD));
  Dec(temp,buf1[0]+1+32*SizeOf(WORD));

  Move(buf1[buf1[0]+1],pattoffs,32*SizeOf(WORD));
  Move(buf1[buf1[0]+32*SizeOf(WORD)+1],buf1,temp);

  temp5 := temp;
  For temp := 0 to 31 do
    begin
      temp2 := 0;
      temp3 := 0;
      If (pattoffs[temp] <> 0) and
         (pattoffs[temp] <= FileSize(f)) then
        Repeat
          temp2 := buf1[pattoffs[temp]-offs0+temp3];
          Repeat
            Inc(temp3);
            temp4 := buf1[pattoffs[temp]-offs0+temp3];
            If (buf1[pattoffs[temp]-offs0+temp3+2] AND $0f <> 0) then
              begin
                If (temp4 AND $0f in [0..8]) then
                  import_rad_event(temp,temp2 AND $3f,temp4 AND $0f,
                                   buf1[pattoffs[temp]-offs0+temp3+1],
                                   buf1[pattoffs[temp]-offs0+temp3+2],
                                   buf1[pattoffs[temp]-offs0+temp3+3]);
                Inc(temp3,3);
              end
            else begin
                   If (temp4 AND $0f in [0..8]) then
                     import_rad_event(temp,temp2 AND $3f,temp4 AND $0f,
                                      buf1[pattoffs[temp]-offs0+temp3+1],
                                      buf1[pattoffs[temp]-offs0+temp3+2],
                                      0);
                   Inc(temp3,2);
                 end;
          until (temp4 OR $80 = temp4) or (temp3 > temp5);
          Inc(temp3);
        until (temp2 OR $80 = temp2) or (temp3 > temp5);
    end;

  CloseF(f);
  songdata_title := NameOnly(songdata_source);
{$IFDEF ADT2PLAY}
  load_flag := 11;
{$ELSE}
  load_flag := 1;
{$ENDIF}

  _dbg_leave; //EXIT //rad_file_loader
end;
