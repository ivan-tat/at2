// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function Fselect(mask: String): String;

var
{$IFNDEF GO32V2}
  temp1: Longint;
{$ENDIF}
  temp2: Longint;
  temp3,temp4: String;
  temp5: Longint;
  temp6,temp7: String;
  temps: String;
  temp8: Longint;
  lastp: Longint;
  idx: Byte;

{$I path_filter.pas}

label _jmp1;

begin
  _dbg_enter ({$I %FILE%}, 'Fselect');

_jmp1:

  idx := 1;
  count := 0;

  Repeat
    temp6 := Upper(ReadChunk(mask,idx));
    Inc(idx,Length(temp6)+1);
    If NOT (temp6 = '') then
      begin
        Inc(count);
        masks[count] := temp6;
      end;
  until (idx >= Length(mask)) or (temp6 = '');

  {$i-}
  GetDir(0,temp6);
  {$i+}

  If (IOresult <> 0) then
    temp6 := fs_environment.last_dir;

  If (fs_environment.last_dir <> '') then
    begin
      {$i-}
      ChDir(fs_environment.last_dir);
      {$i+}
      If (IOresult <> 0) then
        begin
          {$i-}
          ChDir(temp6);
          {$i+}
          If (IOresult <> 0) then ;
          fs_environment.last_file := 'FNAME:EXT';
        end;
    end;

  {$i-}
  GetDir(0,temp3);
  {$i+}
  If (IOresult <> 0) then temp3 := temp6;
  If (temp3[Length(temp3)] <> PATHSEP) then temp3 := temp3+PATHSEP;
  mn_setting.cycle_moves  := FALSE;
  temp4 := '';

  mn_environment.descr_len := 20;
  mn_environment.descr := Addr(descr);
  mn_environment.winshade := FALSE;
  ScreenMemCopy(screen_ptr,ptr_scr_backup);

  Repeat
    path[SUCC(ORD(UpCase(temp3[1]))-ORD('A'))] := path_filter(temp3);
    make_stream(temp3,mask,fstream);

{$IFDEF GO32V2}

    For temp2 := 1 to fstream.count do
      If (fstream.stuff[temp2].name <> UPDIR_STR) then
        begin
          menudat[temp2] := ' '+ExpStrR(BaseNameOnly(
                                   FilterStr2(fstream.stuff[temp2].name,_valid_characters,'_')),8,' ')+' '+
                                 ExpStrR(ExtOnly(
                                   fstream.stuff[temp2].name),3,' ')+' ';
          If (fstream.stuff[temp2].attr AND directory <> 0) then
            menudat[temp2] := iCASE(menudat[temp2]);
        end
      else
        begin
          menudat[temp2] := ExpStrR(' ..',mn_environment.descr_len,' ');
          fstream.stuff[temp2].name := '..';
        end;

    For temp2 := 1 to fstream.count do
      If (fstream.stuff[temp2].attr = volumeid) then
        begin
          If (fstream.stuff[temp2].name = '~'+#255+'~') then descr[temp2] := ''
          else
            descr[temp2] := '[~DRiVE~]';
        end
      else If (fstream.stuff[temp2].attr AND directory <> 0) then
             begin
               If fstream.stuff[temp2].name = '..' then
                 descr[temp2] := ExpStrL('[UP-DiR]',mn_environment.descr_len-1,' ')
               else
                 descr[temp2] := ExpStrL('[DiR]',mn_environment.descr_len-1,' ')
             end
           else
             begin
               temp7 := Num2str(fstream.stuff[temp2].size,10);
               descr[temp2] := '';
               For temp8 := 1 to Length(temp7) do
                 If (temp8 MOD 3 <> 0) or (temp8 = Length(temp7)) then
                   descr[temp2] := temp7[Length(temp7)-temp8+1]+descr[temp2]
                 else
                   descr[temp2] := ','+temp7[Length(temp7)-temp8+1]+descr[temp2];
               descr[temp2] := ExpStrL(descr[temp2],mn_environment.descr_len-1,' ');
             end;

{$ELSE}

    For temp2 := 1 to fstream.count do
      If (fstream.stuff[temp2].attr AND directory <> 0) then
        If (fstream.stuff[temp2].name = UPDIR_STR) then
          begin
            menudat[temp2] := ' '+ExpStrR('..',24,' ')+' ';
            descr[temp2] := ExpStrL('[UP-DiR]',mn_environment.descr_len-1,' ');

            fstream.stuff[temp2].name := '..';
          end
        else
          begin
            temp1 := 24+(mn_environment.descr_len-1-10);
            temp7 := iCASE_filename(DietStr(FilterStr2(fstream.stuff[temp2].name,_valid_characters_fname,'_'),temp1));
            If (Length(temp7) < 24) then
              begin
                menudat[temp2] := ' '+ExpStrR(temp7,24,' ')+' ';
                descr[temp2] := ExpStrR('',mn_environment.descr_len-1-10,' ');
              end
            else
              begin
                menudat[temp2] := ' '+iCASE_filename(ExpStrR(Copy(temp7,1,24),24,' '));
                descr[temp2] := ExpStrR(Copy(temp7,25,Length(temp7)-23),mn_environment.descr_len-1-10,' ');
              end;
            descr[temp2] := descr[temp2]+ExpStrL('[DiR]',10,' ');
          end
      else
        menudat[temp2] := ' '+ExpStrR(DietStr(BaseNameOnly(
                                FilterStr2(fstream.stuff[temp2].name,_valid_characters_fname,'_')),23),23,' ')+' ';

    For temp2 := 1 to fstream.count do
      If (fstream.stuff[temp2].attr = volumeid) then
        begin
          If (fstream.stuff[temp2].name = '~'+#255+'~') then descr[temp2] := ''
          else descr[temp2] := '[~'+fstream.stuff[temp2].info+'~]';
        end
      else If NOT (fstream.stuff[temp2].attr AND directory <> 0) then
             begin
               temp7 := Num2str(fstream.stuff[temp2].size,10);
               descr[temp2] := '';
               For temp8 := 1 to Length(temp7) do
                 If (temp8 MOD 3 <> 0) or (temp8 = Length(temp7)) then
                   descr[temp2] := temp7[Length(temp7)-temp8+1]+descr[temp2]
                 else
                   descr[temp2] := ','+temp7[Length(temp7)-temp8+1]+descr[temp2];
               descr[temp2] := ExpStrR(Copy(ExtOnly(
                                 fstream.stuff[temp2].name),1,3),3,' ')+' '+
                                 ExpStrL(DietStr(descr[temp2],mn_environment.descr_len-1-4),
                                         mn_environment.descr_len-1-4,' ');
             end;

{$ENDIF}

    For temp2 := 1 to fstream.count do
      If (SYSTEM.Pos('~',fstream.stuff[temp2].name) <> 0) and
         (fstream.stuff[temp2].name <> '~'+#255+'~') then
        While (SYSTEM.Pos('~',menudat[temp2]) <> 0) do
          menudat[temp2][SYSTEM.Pos('~',menudat[temp2])] := PATHSEP;

    temp5 := fstream.drive_count+DRIVE_DIVIDER;
    While (temp5 <= fstream.count) and (temp4 <> '') and
          (temp4 <> fstream.stuff[temp5].name) do Inc(temp5);
    If (temp5 > fstream.count) then temp5 := 1;

    For temp2 := 1 to fstream.count do
      If (Lower_filename(fstream.stuff[temp2].name) = fs_environment.last_file) and
         NOT (fstream.stuff[temp2].attr AND volumeid <> 0) then
        begin
          lastp := temp2;
          BREAK;
        end;

    If (Lower_filename(fstream.stuff[temp2].name) <> fs_environment.last_file) then
      lastp := 0;

    If (lastp = 0) or
       (lastp > MAX_FILES) then lastp := temp5;

    mn_setting.reverse_use := TRUE;
    mn_environment.context := ' ~'+Num2str(fstream.match_count,10)+' FiLES FOUND~ ';
    mn_setting.terminate_keys[3] := kBkSPC;
{$IFDEF UNIX}
    mn_setting.terminate_keys[4] := kSlash;
{$ELSE}
    mn_setting.terminate_keys[4] := kSlashR;
{$ENDIF}
    mn_setting.terminate_keys[5] := kF1;
    old_fselect_external_proc := mn_environment.ext_proc;
    mn_environment.ext_proc := new_fselect_external_proc;

    temp := 1;
    While (temp < fstream.count) and (SYSTEM.Pos('[UP-DiR]',descr[temp]) = 0) do Inc(temp);
    If (temp < fstream.count) then mn_setting.homing_pos := temp
    else mn_setting.homing_pos := fstream.drive_count+DRIVE_DIVIDER;

{$IFDEF UNIX}
    Dec(fstream.count);
{$ENDIF}

{$IFDEF GO32V2}
    temp2 := Menu(menudat,01,01,lastp,
                  1+12+1,AdT2unit.max(work_MaxLn-7,30),fstream.count,' '+
                  iCASE(DietStr(path_filter(temp3),28)+' '));
{$ELSE}
    temp2 := Menu(menudat,01,01,lastp,
                  1+23+1,AdT2unit.max(work_MaxLn-5,30),fstream.count,' '+
                  iCASE(DietStr(FilterStr2(path_filter(temp3),_valid_characters_fname,'_'),38))+' ');
{$ENDIF}

    mn_environment.ext_proc := old_fselect_external_proc;
    mn_setting.reverse_use := FALSE;
    mn_environment.context := '';
    mn_setting.terminate_keys[3] := 0;
    mn_setting.terminate_keys[4] := 0;
    mn_setting.terminate_keys[5] := 0;

    If (mn_environment.keystroke = kENTER) and
       (fstream.stuff[temp2].attr AND directory <> 0) then
      begin
        fs_environment.last_file := 'FNAME:EXT';
        mn_environment.keystroke := WORD_NULL;
        If (fstream.stuff[temp2].name = '..') then
          begin
            Delete(temp3,Length(temp3),1);
            temp4 := NameOnly(temp3);
            While (temp3[Length(temp3)] <> PATHSEP) do
              Delete(temp3,Length(temp3),1);
            fs_environment.last_file := Lower_filename(temp4);
          end
        else
          begin
            temp3 := temp3+fstream.stuff[temp2].name+PATHSEP;
            temp4 := '';
            fs_environment.last_file := temp4;
          end;
        {$i-}
        ChDir(Copy(temp3,1,Length(temp3)-1));
        {$i+}
        If (IOresult <> 0) then ;
      end
    else If (mn_environment.keystroke = kENTER) and
            (fstream.stuff[temp2].attr AND volumeid <> 0) then
           begin
             fs_environment.last_file := 'FNAME:EXT';
             mn_environment.keystroke := WORD_NULL;
             {$i-}
             ChDir(path[SUCC(ORD(UpCase(fstream.stuff[temp2].name[1]))-ORD('A'))]);
             {$i+}
             If (IOresult <> 0) then temp3 := path[SUCC(ORD(UpCase(fstream.stuff[temp2].name[1]))-ORD('A'))]
             else begin
                    {$i-}
                    GetDir(0,temp3);
                    {$i+}
                    If (IOresult <> 0) then temp3 := temp6;
                  end;
             If (temp3[Length(temp3)] <> PATHSEP) then temp3 := temp3+PATHSEP;
             temp4 := '';
             fs_environment.last_file := temp4;
           end
         else If (mn_environment.keystroke = kBkSPC) then
                If shift_pressed then
                  begin
                    If (home_dir_path <> '') then
                      temps := home_dir_path
                    else temps := PathOnly(ParamStr(0));
                    If (temps[Length(temps)] <> PATHSEP) then
                      temps := temps+'\';
                    {$i-}
                    ChDir(Copy(temps,1,Length(temps)-1));
                    {$i+}
                    If (IOresult = 0) then
                      begin
                        temp3 := temps;
                        temp4 := '..';
                      end
                    else begin
                           {$i-}
                           ChDir(temp3);
                           {$i+}
                           If (IOresult <> 0) then ;
                         end;
                  end
                else If (SYSTEM.Pos(PATHSEP,Copy(temp3,3,Length(temp3)-3)) <> 0) then
                       begin
                         Delete(temp3,Length(temp3),1);
                         temp4 := NameOnly(temp3);
                         While (temp3[Length(temp3)] <> PATHSEP) do
                           Delete(temp3,Length(temp3),1);
                         fs_environment.last_file := Lower_filename(temp4);
                         {$i-}
                         ChDir(Copy(temp3,1,Length(temp3)-1));
                         {$i+}
                         If (IOresult <> 0) then ;
                       end
                     else
{$IFDEF UNIX}
              else If (mn_environment.keystroke = kSlash) then
{$ELSE}
              else If (mn_environment.keystroke = kSlashR) then
{$ENDIF}
                     begin
                       temp3 := Copy(temp3,1,3);
                       temp4 := '';
                       fs_environment.last_file := temp4;
                       {$i-}
                       ChDir(Copy(temp3,1,Length(temp3)-1));
                       {$i+}
                       If (IOresult <> 0) then ;
                     end
                   else fs_environment.last_file := Lower_filename(fstream.stuff[temp2].name);
  until (mn_environment.keystroke = kENTER) or
        (mn_environment.keystroke = kESC) or
        (mn_environment.keystroke = kF1);

  mn_environment.descr_len := 0;
  mn_environment.descr := NIL;
  mn_environment.winshade := TRUE;
  mn_setting.frame_enabled := TRUE;
  mn_setting.shadow_enabled := TRUE;
  mn_setting.homing_pos := 0;

  move_to_screen_data := ptr_scr_backup;
  move_to_screen_area[1] := mn_environment.xpos;
  move_to_screen_area[2] := mn_environment.ypos;
  move_to_screen_area[3] := mn_environment.xpos+mn_environment.xsize+2+1;
  move_to_screen_area[4] := mn_environment.ypos+mn_environment.ysize+1;
  move2screen;

  If (mn_environment.keystroke = kF1) then
    begin
      HELP('file_browser');
      GOTO _jmp1;
    end;

  Fselect := temp3+fstream.stuff[temp2].name;
  fs_environment.last_dir := path[SUCC(ORD(UpCase(temp3[1]))-ORD('A'))];
  {$i-}
  ChDir(temp6);
  {$i+}
  If (IOresult <> 0) then ;
  If (mn_environment.keystroke = kESC) then Fselect := '';

  _dbg_leave; //EXIT //Fselect
end;
