// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure make_stream(path,mask: String; var stream: tSTREAM);

var
  search: SearchRec;
  count1,count2: Word;
  drive: Char;

{$IFNDEF GO32V2}

{$I ../sdl/CompareStr.pas}

{$ENDIF}

{$I QuickSort.pas}

begin
  _dbg_enter ({$I %FILE%}, 'make_stream');

{$IFNDEF GO32V2}
{$IFNDEF UNIX}
  GetLogicalDriveStrings(SizeOf(drive_list),drive_list);
{$ENDIF}
{$ENDIF}

  count1 := 0;
  For drive := 'A' to 'Z' do
{$IFDEF GO32V2}
    If valid_drive(drive) then
{$ELSE}
    If valid_drive(drive,stream.stuff[SUCC(count1)].info) then
{$ENDIF}
      begin
        Inc(count1);
        stream.stuff[count1].name := drive;
        stream.stuff[count1].attr := volumeid;
        stream.stuff[count1].size := 0;
      end;

  Inc(count1);
  stream.stuff[count1].name := '~'+#255+'~';
  stream.stuff[count1].attr := volumeid;

  count2 := 0;
  stream.drive_count := count1;

{$IFDEF GO32V2}
  If (DiskSize(ORD(UpCase(path[1]))-ORD('A')+1) > 0) then
    begin
{$ENDIF}
      FindFirst(path+WILDCARD_ASTERISK,anyfile-volumeid,search);
      While (DOSerror = 0) and (count1 < MAX_FILES) do
        begin
          If (search.attr AND directory <> 0) and (search.name = '.') then
            begin
              FindNext(search);
              CONTINUE;
            end
          else If (search.attr AND directory <> 0) and
                  NOT ((search.name = '..') and (Length(path) = 3)) then
                 begin
                   If (search.name <> '..') then search.name := search.name
                   else search.name := UPDIR_STR;
                   Inc(count1);
                   stream.stuff[count1].name := search.name;
                   stream.stuff[count1].attr := search.attr;
                 end;
          FindNext(search);
        end;

{$IFNDEF GO32V2}
  If (Length(path) > 3) and (count1 = stream.drive_count) then
    begin
      Inc(count1);
      stream.stuff[count1].name := UPDIR_STR;
      stream.stuff[count1].attr := search.attr;
    end;
{$ENDIF}

      FindFirst(path+WILDCARD_ASTERISK,anyfile-volumeid-directory,search);
      While (DOSerror = 0) and (count1+count2 < MAX_FILES) do
        begin
          If LookUpMask(search.name) then
            begin
          search.name := Lower_filename(search.name);
              Inc(count2);
              stream.stuff[count1+count2].name := search.name;
              stream.stuff[count1+count2].attr := search.attr;
              stream.stuff[count1+count2].size := search.size;
            end;
          FindNext(search);
        end;
{$IFDEF GO32V2}
    end;
{$ENDIF}

  QuickSort(stream.drive_count+DRIVE_DIVIDER,count1);
  QuickSort(count1+DRIVE_DIVIDER,count1+count2);

  stream.count := count1+count2;
  stream.match_count := count2;

  _dbg_leave; //EXIT //make_stream
end;
