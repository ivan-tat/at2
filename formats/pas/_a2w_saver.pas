// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function _a2w_saver: Byte;

const
  id = '_A2insbank_w/macros_';

type
  tHEADER = Record
              ident: array[1..20] of Char;
              crc32: Longint;
              ffver: Byte;
              b0len: Longint;
              b1len: Longint;
              b2len: Longint;
            end;
var
  f: File;
  header: tHEADER;
  temp,temp3: Longint;
  temp_marks: array[1..255] of Char;
  xstart,ystart: Byte;

procedure _restore;
begin
  _dbg_enter ({$I %FILE%}, '_a2w_saver._restore');

  move_to_screen_data := ptr_screen_backup;
  move_to_screen_area[1] := xstart;
  move_to_screen_area[2] := ystart;
  move_to_screen_area[3] := xstart+43+2+1;
  move_to_screen_area[4] := ystart+3+1;
  move2screen;
  progress_num_steps := 0;
  progress_value := 0;

  _dbg_leave; //EXIT //_a2w_saver._restore
end;

begin
  _dbg_enter ({$I %FILE%}, '_a2w_saver');

  _a2w_saver := 0;
  {$i-}
  Assign(f,instdata_source);
  ResetF(f);
  {$i+}
  If (IOresult = 0) and NOT quick_cmd then
    begin
      If (dl_environment.keystroke = kESC) then
        begin
          CloseF(f);
          _dbg_leave; EXIT; //_a2w_saver
        end;
        temp := Dialog('FiLE "'+iCASE(NameOnly(instdata_source))+
                       '" ALREADY EXiSTS iN DESTiNATiON DiRECTORY$',
                       '~O~VERWRiTE$~R~ENAME$~C~ANCEL$',' A2W SAVER ',1);

      If ((dl_environment.keystroke <> kESC) and (temp = 3)) or
          (dl_environment.keystroke = kESC) then
        begin
          CloseF(f);
          _dbg_leave; EXIT; //_a2w_saver
        end
      else If (dl_environment.keystroke <> kESC) and (temp = 2) then
             begin
               CloseF(f);
               _a2w_saver := BYTE_NULL;
               _dbg_leave; EXIT; //_a2w_saver
             end;
    end
  else If (IOresult <> 0) then
         begin
           CloseF(f);
           Dialog('ERROR WRiTiNG DATA - DiSK ERROR?$'+
                  'SAViNG STOPPED$',
                  '~O~KAY$',' A2W SAVER ',1);
           _dbg_leave; EXIT; //_a2w_saver
         end;
  {$i-}
  RewriteF(f);
  {$i+}
  If (IOresult <> 0) then
    begin
      CloseF(f);
      Dialog('ERROR WRiTiNG DATA - DiSK ERROR?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2W SAVER ',1);
      _dbg_leave; EXIT; //_a2w_saver
    end;

  ScreenMemCopy(screen_ptr,ptr_screen_backup);
  centered_frame_vdest := screen_ptr;
  HideCursor;

  centered_frame(xstart,ystart,43,3,' A2W FiLE ',
                 dialog_background+dialog_border,
                 dialog_background+dialog_title,
                 frame_double);

  progress_xstart := xstart+2;
  progress_ystart := ystart+2;
  progress_step := 1;
  progress_num_steps := 3;

  ShowCStr(screen_ptr,xstart+2,ystart+1,
           'COMPRESSiNG iNSTRUMENT BANK DATA...',
           dialog_background+dialog_text,
           dialog_background+dialog_hi_text);
  show_progress(DWORD_NULL);

  header.crc32 := DWORD_NULL;
  header.ident := id;
  header.ffver := FFVER_A2W;

  BlockWriteF(f,header,SizeOf(header),temp);
  If NOT (temp = SizeOf(header)) then
    begin
      CloseF(f);
      EraseF(f);
      Dialog('ERROR WRiTiNG DATA - DiSK FULL?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2W SAVER ',1);
      _dbg_leave; EXIT; //_a2w_saver
    end;

  For temp := 1 to 255 do
    begin
      temp_marks[temp] := songdata.instr_names[temp][1];
      Delete(songdata.instr_names[temp],1,9);
      FillChar(songdata.instr_names[temp][SUCC(Length(songdata.instr_names[temp]))],
               32-Length(songdata.instr_names[temp]),0);
    end;

  temp3 := SizeOf(songdata.instr_names)+
           SizeOf(songdata.instr_data)+
           SizeOf(songdata.instr_macros);
  Move(songdata.instr_names,buf2,temp3);
  Move(songdata.ins_4op_flags,buf2[temp3],SizeOf(songdata.ins_4op_flags));
  Inc(temp3,SizeOf(songdata.ins_4op_flags));
  header.b0len := LZH_compress(buf2,buf1,temp3);
  Inc(progress_step);
  For temp := 1 to 255 do
    Insert(temp_marks[temp]+
           'iNS_'+byte2hex(temp)+#247' ',
           songdata.instr_names[temp],1);

  BlockWriteF(f,buf1,header.b0len,temp);
  If NOT (temp = header.b0len) then
    begin
      CloseF(f);
      EraseF(f);
      Dialog('ERROR WRiTiNG DATA - DiSK FULL?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2W SAVER ',1);
      _dbg_leave; EXIT; //_a2w_saver
    end;

  header.crc32 := Update32(buf1,header.b0len,header.crc32);
  header.b1len := LZH_compress(songdata.macro_table,buf1,SizeOf(songdata.macro_table));
  Inc(progress_step);
  BlockWriteF(f,buf1,header.b1len,temp);
  If NOT (temp = header.b1len) then
    begin
      CloseF(f);
      EraseF(f);
      Dialog('ERROR WRiTiNG DATA - DiSK FULL?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2W SAVER ',1);
      _dbg_leave; EXIT; //_a2w_saver
    end;

  header.crc32 := Update32(buf1,header.b1len,header.crc32);
  header.b2len := LZH_compress(songdata.dis_fmreg_col,buf1,SizeOf(songdata.dis_fmreg_col));
  Inc(progress_step);
  BlockWriteF(f,buf1,header.b2len,temp);
  If NOT (temp = header.b2len) then
    begin
      CloseF(f);
      EraseF(f);
      Dialog('ERROR WRiTiNG DATA - DiSK FULL?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2W SAVER ',1);
      _dbg_leave; EXIT; //_a2w_saver
    end;

  header.crc32 := Update32(buf1,header.b2len,header.crc32);
  header.crc32 := Update32(header.b0len,2,header.crc32);
  header.crc32 := Update32(header.b1len,2,header.crc32);
  header.crc32 := Update32(header.b2len,2,header.crc32);

  ResetF_RW(f);
  BlockWriteF(f,header,SizeOf(header),temp);
  If NOT (temp = SizeOf(header)) then
    begin
      CloseF(f);
      EraseF(f);
      Dialog('ERROR WRiTiNG DATA - DiSK FULL?$'+
             'SAViNG STOPPED$',
             '~O~KAY$',' A2W SAVER ',1);
      _dbg_leave; EXIT; //_a2w_saver
    end;

  CloseF(f);
  progress_num_steps := 1;
  progress_step := 1;
  progress_value := 1;
  progress_old_value := BYTE_NULL;
  _draw_screen_without_delay := TRUE;
  show_progress(1);
  // delay for awhile to show progress bar at 100%
{$IFDEF GO32V2}
  CRT.Delay(500);
{$ELSE}
  SDL_Delay(200);
{$ENDIF}
  _restore;

  _dbg_leave; //EXIT //_a2w_saver
end;
