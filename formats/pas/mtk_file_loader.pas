// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

type
  tMTK_DATA = Record
                sname: String[33];
                compo: String[33];
                instn: array[0..$7f] of String[33];
                instt: array[0..$7f] of array[0..$0b] of Byte;
                order: array[0..$7f] of Byte;
                patts: tHSC_PATTERNS;
                dummy: Byte;
              end;

var
  buffer2: tMTK_DATA;

procedure mtk_file_loader;

var
  f: File;
  temp,temp2: Longint;
  crc: Word;
  old_c_fix: Boolean;

const
  id = 'mpu401tr'#146'kk'#238'r@data';

var
  header: Record
            id_string: array[1..18] of Char;
            crc_16bit: Word;
            data_size: Word;
          end;
begin
  _dbg_enter ({$I %FILE%}, 'mtk_file_loader');

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
             '~O~KAY$',' MTK LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //mtk_file_loader
    end;

  BlockReadF(f,header,SizeOf(header),temp);
  If NOT ((temp = SizeOf(header)) and (header.id_string = id)) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' MTK LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //mtk_file_loader
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

  crc := 0;
  crc := Update16(buf1,temp,crc);
  If (crc <> header.crc_16bit) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('CRC FAiLED - FiLE CORRUPTED$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' MTK LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //mtk_file_loader
    end;

  FillChar(buffer2,SizeOf(buffer2),0);
  temp2 := RDC_decompress(buf1,buffer2,temp);
  If NOT (temp2 = header.data_size) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' MTK LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //mtk_file_loader
    end;

  init_songdata;
  load_flag := 0;

  songdata.patt_len := 64;
  If adjust_tracks then songdata.nm_tracks := 9
  else If (songdata.nm_tracks < 9) then songdata.nm_tracks := 9;

  tempo := 18;
  speed := 2;

  songdata.common_flag := songdata.common_flag OR 2;
  songdata.tempo := tempo;
  songdata.speed := speed;
  import_old_flags;

  For temp2 := 0 to $31 do
    If (buffer2.order[temp2] <> $ff) then songdata.pattern_order[temp2] := buffer2.order[temp2]
    else songdata.pattern_order[temp2] := $080;

  old_c_fix := fix_c_note_bug;
  fix_c_note_bug := FALSE;
  import_hsc_patterns(buffer2.patts,
                     (header.data_size-SizeOf(buffer2.sname)
                                      -SizeOf(buffer2.compo)
                                      -SizeOf(buffer2.instn)
                                      -SizeOf(buffer2.instt)
                                      -SizeOf(buffer2.order)-1) DIV $480);
  fix_c_note_bug := old_c_fix;

// specific corrections for MPU-401 TRAKKER instrument
  For temp2 := 0 to $7f do
    begin
      import_hsc_instrument_alt (songdata.instr_data[temp2+1], buffer2.instt[temp2]);
      With songdata.instr_data[temp2+1].fm_data do
        begin
          If (KSL_VOLUM_modulator > 128) then
            KSL_VOLUM_modulator := KSL_VOLUM_modulator DIV 3;
          If (KSL_VOLUM_carrier > 128) then
            KSL_VOLUM_carrier := KSL_VOLUM_carrier DIV 3;
        end;

      songdata.instr_names[temp2+1] :=
        Copy(songdata.instr_names[temp2+1],1,9)+
        truncate_string(Copy(buffer2.instn[temp2],10,32));
    end;

  songdata.songname := CutStr(buffer2.sname);
  songdata.composer := CutStr(buffer2.compo);

  CloseF(f);
  songdata_title := NameOnly(songdata_source);
{$IFDEF ADT2PLAY}
  load_flag := 10;
{$ELSE}
  load_flag := 1;
{$ENDIF}

  _dbg_leave; //EXIT //mtk_file_loader
end;
