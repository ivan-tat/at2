// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function _a2t_saver: Byte;

type
  tHEADER = Record
              ident: array[1..15] of Char;
              crc32: Longint;
              ffver: Byte;
              patts: Byte;
              tempo: Byte;
              speed: Byte;
              cflag: Byte;
              patln: Word;
              nmtrk: Byte;
              mcspd: Word;
              is4op: Byte;
              locks: array[1..20] of Byte;
              b0len: Longint;
              b1len: Longint;
              b2len: Longint;
              b3len: Longint;
              b4len: Longint;
              b5len: array[0..15] of Longint;
            end;
const
  id = '_A2tiny_module_';

var
  f: File;
  header: tHEADER;
  instruments: Byte;
  temp,temp2,index: Longint;
  temps: String;
  xstart,ystart: Byte;

procedure _restore;
begin
  _dbg_enter ({$I %FILE%}, '_a2t_saver._restore');

  move_to_screen_data := ptr_screen_backup;
  move_to_screen_area[1] := xstart;
  move_to_screen_area[2] := ystart;
  move_to_screen_area[3] := xstart+43+2+1;
  move_to_screen_area[4] := ystart+3+1;
  move2screen;
  progress_num_steps := 0;
  progress_value := 0;

  _dbg_leave; //EXIT //_a2t_saver._restore
end;

begin
  _dbg_enter ({$I %FILE%}, '_a2t_saver');

  _a2t_saver := 0;
  {$i-}
  Assign(f,songdata_source);
  ResetF(f);
  {$i+}
  If (IOresult = 0) and NOT quick_cmd then
    begin
      If (dl_environment.keystroke = kESC) then
        begin
          CloseF(f);
          _dbg_leave; EXIT; //_a2t_saver
        end;
        temp := Dialog('FiLE "'+iCASE(NameOnly(songdata_source))+
                       '" ALREADY EXiSTS iN DESTiNATiON DiRECTORY$',
                       '~O~VERWRiTE$~R~ENAME$~C~ANCEL$',' A2T SAVER ',1);

      If ((dl_environment.keystroke <> kESC) and (temp = 3)) or
          (dl_environment.keystroke = kESC) then
        begin
          CloseF(f);
          _dbg_leave; EXIT; //_a2t_saver
        end
      else If (dl_environment.keystroke <> kESC) and (temp = 2) then
             begin
               CloseF(f);
               _a2t_saver := BYTE_NULL;
               _dbg_leave; EXIT; //_a2t_saver
             end;
    end
  else If (IOresult <> 0) then
         begin
           CloseF(f);
           Dialog('ERROR WRiTiNG DATA - DiSK ERROR?'+
                  'SAViNG STOPPED$',
                  '~O~KAY$',' A2T SAVER ',1);
           _dbg_leave; EXIT; //_a2t_saver
         end;
  {$i-}
  RewriteF(f);
  {$i+}
  If (IOresult <> 0) then
    begin
      CloseF(f);
      Dialog('ERROR WRiTiNG DATA - DiSK ERROR?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2T SAVER ',1);
      _dbg_leave; EXIT; //_a2t_saver
    end;

  FillChar(header,SizeOf(header),0);
  count_patterns(header.patts);
  header.crc32 := DWORD_NULL;
  header.ident := id;
  header.tempo := songdata.tempo;
  header.speed := songdata.speed;
  header.cflag := BYTE(speed_update)+BYTE(lockvol) SHL 1+
                                     BYTE(lockVP)  SHL 2+
                                     tremolo_depth SHL 3+
                                     vibrato_depth SHL 4+
                                     BYTE(panlock) SHL 5+
                                     BYTE(percussion_mode) SHL 6+
                                     BYTE(volume_scaling) SHL 7;

  songdata.bpm_data.rows_per_beat := mark_line;
  songdata.bpm_data.tempo_finetune := IRQ_freq_shift;

  header.patln := songdata.patt_len;
  header.nmtrk := songdata.nm_tracks;
  header.mcspd := songdata.macro_speedup;
  header.is4op := songdata.flag_4op;
  Move(songdata.lock_flags,header.locks,SizeOf(header.locks));
  header.ffver := FFVER_A2T;

  BlockWriteF(f,header,SizeOf(header),temp);
  If NOT (temp = SizeOf(header)) then
    begin
      CloseF(f);
      EraseF(f);
      Dialog('ERROR WRiTiNG DATA - DiSK FULL?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2T SAVER ',1);
      _dbg_leave; EXIT; //_a2t_saver
    end;

  ScreenMemCopy(screen_ptr,ptr_screen_backup);
  centered_frame_vdest := screen_ptr;
  HideCursor;
  If (header.patts = 0) then header.patts := 1;

  temps := 'A2T FiLE';
  centered_frame(xstart,ystart,43,3,' '+temps+' ',
                 dialog_background+dialog_border,
                 dialog_background+dialog_title,
                 frame_double);

  progress_xstart := xstart+2;
  progress_ystart := ystart+2;
  progress_step := 1;
  progress_num_steps := (header.patts-1) DIV 8 +6;

  ShowCStr(screen_ptr,xstart+2,ystart+1,
           'COMPRESSiNG TiNY MODULE DATA...',
           dialog_background+dialog_text,
           dialog_background+dialog_hi_text);
  show_progress(DWORD_NULL);

  count_instruments(instruments);
  instruments := min(instruments,1);
  temp2 := 0;
  Move(songdata.bpm_data,buf2[temp2],SizeOf(songdata.bpm_data));
  Inc(temp2,SizeOf(songdata.bpm_data));
  Move(songdata.ins_4op_flags,buf2[temp2],SizeOf(songdata.ins_4op_flags));
  Inc(temp2,SizeOf(songdata.ins_4op_flags));
  Move(songdata.reserved_data,buf2[temp2],SizeOf(songdata.reserved_data));
  Inc(temp2,SizeOf(songdata.reserved_data));
  Move(songdata.instr_data,buf2[temp2],instruments*SizeOf(songdata.instr_data[1]));
  Inc(temp2,instruments*SizeOf(songdata.instr_data[1]));
  header.b0len := LZH_compress_ultra(buf2,buf1,temp2);
  BlockWriteF(f,buf1,header.b0len,temp);
  Inc(progress_step);
  If NOT (temp = header.b0len) then
    begin
      CloseF(f);
      EraseF(f);
      _restore;
      Dialog('ERROR WRiTiNG DATA - DiSK FULL?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2T SAVER ',1);
      _dbg_leave; EXIT; //_a2t_saver
    end;

  header.crc32 := Update32(buf1,header.b0len,header.crc32);
  temp := 0;
  temp2 := instruments*SizeOf(songdata.instr_macros[1]);
  If NOT Empty(songdata.instr_macros,temp2) then
    begin
      header.b1len := LZH_compress_ultra(songdata.instr_macros,buf1,temp2);
      BlockWriteF(f,buf1,header.b1len,temp);
    end
  else header.b1len := 0;
  Inc(progress_step);
  If NOT (temp = header.b1len) then
    begin
      CloseF(f);
      EraseF(f);
      _restore;
      Dialog('ERROR WRiTiNG DATA - DiSK FULL?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2T SAVER ',1);
      _dbg_leave; EXIT; //_a2t_saver
    end;

  header.crc32 := Update32(buf1,header.b1len,header.crc32);
  temp := 0;
  temp2 := SizeOf(songdata.macro_table);
  If NOT Empty(songdata.macro_table,temp2) then
    begin
      header.b2len := LZH_compress_ultra(songdata.macro_table,buf1,temp2);
      BlockWriteF(f,buf1,header.b2len,temp);
    end
  else header.b2len := 0;
  Inc(progress_step);
  If NOT (temp = header.b2len) then
    begin
      CloseF(f);
      EraseF(f);
      _restore;
      Dialog('ERROR WRiTiNG DATA - DiSK FULL?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2T SAVER ',1);
      _dbg_leave; EXIT; //_a2t_saver
    end;

  header.crc32 := Update32(buf1,header.b2len,header.crc32);
  temp := 0;
  temp2 := SizeOf(songdata.dis_fmreg_col);
  If NOT Empty(songdata.dis_fmreg_col,temp2) then
    begin
      header.b3len := LZH_compress_ultra(songdata.dis_fmreg_col,buf1,temp2);
      BlockWriteF(f,buf1,header.b3len,temp);
    end
  else header.b3len := 0;
  Inc(progress_step);
  If NOT (temp = header.b3len) then
    begin
      CloseF(f);
      EraseF(f);
      _restore;
      Dialog('ERROR WRiTiNG DATA - DiSK FULL?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2T SAVER ',1);
      _dbg_leave; EXIT; //_a2t_saver
    end;

  header.crc32 := Update32(buf1,header.b3len,header.crc32);
  temp2 := SizeOf(songdata.pattern_order);
  header.b4len := LZH_compress_ultra(songdata.pattern_order,buf1,temp2);
  BlockWriteF(f,buf1,header.b4len,temp);
  Inc(progress_step);
  If NOT (temp = header.b4len) then
    begin
      CloseF(f);
      EraseF(f);
      _restore;
      Dialog('ERROR WRiTiNG DATA - DiSK FULL?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2T SAVER ',1);
      _dbg_leave; EXIT; //_a2t_saver
    end;

  header.crc32 := Update32(buf1,header.b4len,header.crc32);
  If (header.patts < 1*8) then temp2 := header.patts*SizeOf(pattdata^[0][0])
  else temp2 := SizeOf(pattdata^[0]);

  header.b5len[0] := LZH_compress_ultra(pattdata^[0],buf1,temp2);
  BlockWriteF(f,buf1,header.b5len[0],temp);
  Inc(progress_step);
  If NOT (temp = header.b5len[0]) then
    begin
      CloseF(f);
      EraseF(f);
      _restore;
      Dialog('ERROR WRiTiNG DATA - DiSK FULL?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2T SAVER ',1);
      _dbg_leave; EXIT; //_a2t_saver
    end;

  header.crc32 := Update32(buf1,header.b5len[0],header.crc32);
  For index := 1 to 15 do
    If ((header.patts-1) DIV 8 > PRED(index)) then
      begin
        If (header.patts < SUCC(index)*8) then
          temp2 := (header.patts-index*8)*SizeOf(pattdata^[index][0])
        else temp2 := SizeOf(pattdata^[index]);
        header.b5len[index] := LZH_compress_ultra(pattdata^[index],buf1,temp2);
        BlockWriteF(f,buf1,header.b5len[index],temp);
        Inc(progress_step);
        If NOT (temp = header.b5len[index]) then
          begin
            CloseF(f);
            EraseF(f);
            _restore;
            Dialog('ERROR WRiTiNG DATA - DiSK FULL?$'+
                   'SAViNG STOPPED$',
                   '~O~KAY$',' A2T SAVER ',1);
            _dbg_leave; EXIT; //_a2t_saver
          end;
        header.crc32 := Update32(buf1,header.b5len[index],header.crc32);
      end;

  header.crc32 := Update32(header.b0len,2,header.crc32);
  header.crc32 := Update32(header.b1len,2,header.crc32);
  header.crc32 := Update32(header.b2len,2,header.crc32);
  header.crc32 := Update32(header.b3len,2,header.crc32);
  header.crc32 := Update32(header.b4len,2,header.crc32);

  For index := 0 to 15 do
    header.crc32 := Update32(header.b5len[index],2,header.crc32);

  ResetF_RW(f);
  BlockWriteF(f,header,SizeOf(header),temp);
  If NOT (temp = SizeOf(header)) then
    begin
      CloseF(f);
      EraseF(f);
      _restore;
      Dialog('ERROR WRiTiNG DATA - DiSK FULL?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2T SAVER ',1);
      _dbg_leave; EXIT; //_a2t_saver
    end;

  CloseF(f);
  _restore;
  songdata_title := NameOnly(songdata_source);
  songdata_crc := Update32(songdata,SizeOf(songdata),0);
  songdata_crc_ord := Update32(songdata.pattern_order,
                               SizeOf(songdata.pattern_order),0);
  module_archived := TRUE;

  _dbg_leave; //EXIT //_a2t_saver
end;
