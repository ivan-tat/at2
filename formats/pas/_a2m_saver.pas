// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function _a2m_saver: Byte;

type
  tHEADER = Record
              ident: array[1..10] of Char;
              crc32: Longint;
              ffver: Byte;
              patts: Byte;
              b0len: Longint;
              b1len: array[0..15] of Longint;
            end;
const
  id = '_A2module_';

var
  f: File;
  header: tHEADER;
  temp,index: Longint;
  xstart,ystart: Byte;
  temp_marks: array[1..255] of Char;
  temp_marks2: array[0..$7f] of Char;

procedure _restore;
begin
  _dbg_enter ({$I %FILE%}, '_a2m_saver._restore');

  move_to_screen_data := ptr_screen_backup;
  move_to_screen_area[1] := xstart;
  move_to_screen_area[2] := ystart;
  move_to_screen_area[3] := xstart+43+2+1;
  move_to_screen_area[4] := ystart+3+1;
  move2screen;
  progress_num_steps := 0;
  progress_value := 0;

  _dbg_leave; //EXIT //_a2m_saver._restore
end;

begin
  _dbg_enter ({$I %FILE%}, '_a2m_saver');

  _a2m_saver := 0;
  {$i-}
  Assign(f,songdata_source);
  ResetF(f);
  {$i+}
  If (IOresult = 0) and NOT quick_cmd then
    begin
      If (dl_environment.keystroke = kESC) then
        begin
          CloseF(f);
          _dbg_leave; EXIT; //_a2m_saver
        end;
        temp := Dialog('FiLE "'+iCASE(NameOnly(songdata_source))+
                       '" ALREADY EXiSTS iN DESTiNATiON DiRECTORY$',
                       '~O~VERWRiTE$~R~ENAME$~C~ANCEL$',' A2M SAVER ',1);

      If ((dl_environment.keystroke <> kESC) and (temp = 3)) or
          (dl_environment.keystroke = kESC) then
        begin
          CloseF(f);
          _dbg_leave; EXIT; //_a2m_saver
        end
      else If (dl_environment.keystroke <> kESC) and (temp = 2) then
             begin
               CloseF(f);
               _a2m_saver := BYTE_NULL;
               _dbg_leave; EXIT; //_a2m_saver
             end;
    end
  else If (IOresult <> 0) then
         begin
           CloseF(f);
           Dialog('ERROR WRiTiNG DATA - DiSK ERROR?$'+
                  'SAViNG STOPPED$',
                  '~O~KAY$',' A2M SAVER ',1);
           _dbg_leave; EXIT; //_a2m_saver
         end;
  {$i-}
  RewriteF(f);
  {$i+}
  If (IOresult <> 0) then
    begin
      CloseF(f);
      Dialog('ERROR WRiTiNG DATA - DiSK ERROR?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2M SAVER ',1);
      _dbg_leave; EXIT; //_a2m_saver
    end;

  FillChar(songdata.songname[SUCC(Length(songdata.songname))],
            PRED(SizeOf(songdata.songname))-
            Length(songdata.songname),0);

  FillChar(songdata.composer[SUCC(Length(songdata.composer))],
            PRED(SizeOf(songdata.composer))-
            Length(songdata.composer),0);

  For temp := 1 to 255 do
    FillChar(songdata.instr_names[temp][SUCC(Length(songdata.instr_names[temp]))],
              PRED(SizeOf(songdata.instr_names[temp]))-
              Length(songdata.instr_names[temp]),0);

  For temp := 0 to $7f do
    FillChar(songdata.pattern_names[temp][SUCC(Length(songdata.pattern_names[temp]))],
              PRED(SizeOf(songdata.pattern_names[temp]))-
              Length(songdata.pattern_names[temp]),0);

  FillChar(header,SizeOf(header),0);
  count_patterns(header.patts);
  header.crc32 := DWORD_NULL;
  header.ident := id;

  songdata.common_flag := BYTE(speed_update)+BYTE(lockvol) SHL 1+
                                             BYTE(lockVP)  SHL 2+
                                             tremolo_depth SHL 3+
                                             vibrato_depth SHL 4+
                                             BYTE(panlock) SHL 5+
                                             BYTE(percussion_mode) SHL 6+
                                             BYTE(volume_scaling) SHL 7;

  songdata.bpm_data.rows_per_beat := mark_line;
  songdata.bpm_data.tempo_finetune := IRQ_freq_shift;

  header.ffver := FFVER_A2M;
  BlockWriteF(f,header,SizeOf(header),temp);
  If NOT (temp = SizeOf(header)) then
    begin
      CloseF(f);
      EraseF(f);
      Dialog('ERROR WRiTiNG DATA - DiSK FULL?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2M SAVER ',1);
      _dbg_leave; EXIT; //_a2m_saver
    end;

  ScreenMemCopy(screen_ptr,ptr_screen_backup);
  centered_frame_vdest := screen_ptr;
  HideCursor;
  If (header.patts = 0) then header.patts := 1;

  centered_frame(xstart,ystart,43,3,' A2M FiLE ',
                 dialog_background+dialog_border,
                 dialog_background+dialog_title,
                 frame_double);

  progress_xstart := xstart+2;
  progress_ystart := ystart+2;
  progress_step := 1;
  progress_num_steps := (header.patts-1) DIV 8 +2;

  ShowCStr(screen_ptr,xstart+2,ystart+1,
           'COMPRESSiNG MODULE DATA...',
           dialog_background+dialog_text,
           dialog_background+dialog_hi_text);
  show_progress(DWORD_NULL);

  For temp := 1 to 255 do
    begin
      temp_marks[temp] := songdata.instr_names[temp][1];
      Delete(songdata.instr_names[temp],1,9);
      FillChar(songdata.instr_names[temp][SUCC(Length(songdata.instr_names[temp]))],
               32-Length(songdata.instr_names[temp]),0);
    end;

  For temp := 0 to $7f do
    begin
      temp_marks2[temp] := songdata.pattern_names[temp][1];
      Delete(songdata.pattern_names[temp],1,11);
      FillChar(songdata.pattern_names[temp][SUCC(Length(songdata.pattern_names[temp]))],
               32-Length(songdata.pattern_names[temp]),0);
    end;

  header.b0len := LZH_compress(songdata,buf1,SizeOf(songdata));
  For temp := 1 to 255 do
    Insert(temp_marks[temp]+
           'iNS_'+byte2hex(temp)+#247' ',
           songdata.instr_names[temp],1);

  For temp := 0 to $7f do
    Insert(temp_marks2[temp]+
           'PAT_'+byte2hex(temp)+'  '#247' ',
           songdata.pattern_names[temp],1);

  BlockWriteF(f,buf1,header.b0len,temp);
  Inc(progress_step);
  If NOT (temp = header.b0len) then
    begin
      CloseF(f);
      EraseF(f);
      _restore;
      Dialog('ERROR WRiTiNG DATA - DiSK FULL?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2M SAVER ',1);
      _dbg_leave; EXIT; //_a2m_saver
    end;

  header.crc32 := Update32(buf1,header.b0len,header.crc32);
  header.b1len[0] := LZH_compress(pattdata^[0],buf1,SizeOf(pattdata^[0]));
  BlockWriteF(f,buf1,header.b1len[0],temp);
  Inc(progress_step);
  If NOT (temp = header.b1len[0]) then
    begin
      CloseF(f);
      EraseF(f);
      _restore;
      Dialog('ERROR WRiTiNG DATA - DiSK FULL?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2M SAVER ',1);
      _dbg_leave; EXIT; //_a2m_saver
    end;

  header.crc32 := Update32(buf1,header.b1len[0],header.crc32);
  For index := 1 to 15 do
    If ((header.patts-1) DIV 8 > PRED(index)) then
      begin
        header.b1len[index] := LZH_compress(pattdata^[index],buf1,SizeOf(pattdata^[index]));
        BlockWriteF(f,buf1,header.b1len[index],temp);
        Inc(progress_step);
        If NOT (temp = header.b1len[index]) then
          begin
            CloseF(f);
            EraseF(f);
            _restore;
            Dialog('ERROR WRiTiNG DATA - DiSK FULL?$'+
                   'SAViNG STOPPED$',
                   '~O~KAY$',' A2M SAVER ',1);
            _dbg_leave; EXIT; //_a2m_saver
          end;
        header.crc32 := Update32(buf1,header.b1len[index],header.crc32);
      end;

  header.crc32 := Update32(header.b0len,2,header.crc32);
  For index := 0 to 15 do
    header.crc32 := Update32(header.b1len[index],2,header.crc32);

  ResetF_RW(f);
  BlockWriteF(f,header,SizeOf(header),temp);
  If NOT (temp = SizeOf(header)) then
    begin
      CloseF(f);
      EraseF(f);
      _restore;
      Dialog('ERROR WRiTiNG DATA - DiSK FULL?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2M SAVER ',1);
      _dbg_leave; EXIT; //_a2m_saver
    end;

  CloseF(f);
  _restore;
  songdata_title := NameOnly(songdata_source);
  songdata_crc := Update32(songdata,SizeOf(songdata),0);
  songdata_crc_ord := Update32(songdata.pattern_order,
                               SizeOf(songdata.pattern_order),0);
  module_archived := TRUE;

  _dbg_leave; //EXIT //_a2m_saver
end;
