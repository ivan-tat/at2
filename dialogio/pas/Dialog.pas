// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function Dialog(text,keys,title: String; spos: Byte): Byte;

{$I Dialog_SubPos.pas}
{$I Dialog_AddPos.pas}
{$I Dialog_ShowItem.pas}
{$I Dialog_RetKey.pas}
{$I Dialog_CurrentKey.pas}

begin
  _dbg_enter ({$I %FILE%}, 'Dialog');

  pos := 1;
  max := Length(title);
  num := 0;

  ScreenMemCopy(screen_ptr,ptr_scr_backup);
  HideCursor;

  Repeat
    str := ReadChunk(text,pos);
    Inc(pos,Length(str)+1);
    If (CStrLen(str) > max) then max := CStrLen(str);
    If (str <> '') then Inc(num);
  until (pos >= Length(text)) or (str = '');

  pos := 1;
  mx2 := 0;
  nm2 := 0;

  If (Copy(keys,1,14) = '%string_input%') then
    begin
      Inc(pos,14);
      str := ReadChunk(keys,pos); ln := Str2num(str,10);
      If (str = '') then
        begin
          _dbg_leave; EXIT; //Dialog
        end;
      Inc(pos,Length(str)+1);

      str := ReadChunk(keys,pos); ln1 := Str2num(str,10); mx2 := ln1;
      If (str = '') then
        begin
          _dbg_leave; EXIT; //Dialog
        end;
      Inc(pos,Length(str)+1);

      str := ReadChunk(keys,pos); atr1 := Str2num(str,16);
      If (str = '') then
        begin
          _dbg_leave; EXIT; //Dialog
        end;
      Inc(pos,Length(str)+1);

      str := ReadChunk(keys,pos); atr2 := Str2num(str,16);
      If (str = '') then
        begin
          _dbg_leave; EXIT; //Dialog
        end;
      Inc(pos,Length(str)+1);
    end
  else
    begin
      Repeat
        str := ReadChunk(keys,pos);
        Inc(pos,Length(str)+1);
        If (str <> '') then
          begin
            Inc(nm2);
            dbuf[nm2].str := ' '+str+' ';
            dbuf[nm2].key := OutKey(str);
            If NOT dl_setting.all_enabled then dbuf[nm2].use := dbuf[nm2].key <> '~'
            else dbuf[nm2].use := TRUE;
            If (nm2 > 1) then
              begin
                dbuf[nm2].pos := dbuf[nm2-1].pos+CStrLen(dbuf[nm2-1].str)+1;
                Inc(mx2,CStrLen(dbuf[nm2].str)+1);
              end
            else
              begin
                dbuf[nm2].pos := 1;
                Inc(mx2,CStrLen(dbuf[nm2].str));
              end;
          end;
      until (pos >= Length(keys)) or (str = '');
    end;

  If (max < mx2) then max := mx2
  else
    begin
      ln1 := max;
      If (ln < max) then ln := max;
    end;

  If dl_setting.center_box then
    begin
      xstart := (work_MaxCol-(max+4)) DIV 2+(work_MaxCol-(max+4)) MOD 2;
      ystart := (work_MaxLn -(num+2)) DIV 2+(work_MaxLn -(num+2)) MOD 2;
    end
  else
    begin
      xstart := dl_setting.xstart;
      ystart := dl_setting.ystart;
    end;

  old_fr_shadow_enabled := fr_setting.shadow_enabled;
  fr_setting.shadow_enabled := dl_setting.shadow_enabled;
  Frame(screen_ptr,xstart,ystart,xstart+max+3,ystart+num+2,
        dl_setting.box_attr,title,dl_setting.title_attr,
        dl_setting.frame_type);
  fr_setting.shadow_enabled := old_fr_shadow_enabled;

  dl_environment.xpos  := xstart;
  dl_environment.ypos  := ystart;
  dl_environment.xsize := max+3;
  dl_environment.ysize := num+2;

  pos := 1;
  contxt := DietStr(dl_environment.context,max+
    (Length(dl_environment.context)-CStrLen(dl_environment.context)));
  ShowCStr(screen_ptr,xstart+max+3-CStrLen(contxt),ystart+num+2,
           contxt,dl_setting.contxt_attr,dl_setting.contxt2_attr);

  For idx := 1 to num do
    begin
      str := ReadChunk(text,pos);
      Inc(pos,Length(str)+1);
      If dl_setting.center_text then
        ShowCStr(screen_ptr,xstart+2,ystart+idx,
                 ExpStrL(str,Length(str)+(max-CStrLen(str)) DIV 2,' '),
                 dl_setting.text_attr,dl_setting.text2_attr)
      else
        ShowCStr(screen_ptr,xstart+2,ystart+idx,
                 str,dl_setting.text_attr,dl_setting.text2_attr);
    end;

  If (Copy(keys,1,14) = '%string_input%') then
    begin
      If use_large_cursor then WideCursor
      else ThinCursor;
      str := InputStr(dl_environment.input_str,
                      xstart+2,ystart+num+1,ln,ln1,atr1,atr2);
      If is_environment.keystroke = kENTER then dl_environment.input_str := str;
      dl_environment.keystroke := is_environment.keystroke;
      HideCursor;
    end
  else
    begin
      For idx := 1 to nm2 do
        begin
          Inc(dbuf[idx].pos,xstart+(max-mx2) DIV 2+1);
          If dbuf[idx].use then
            ShowCStr(screen_ptr,dbuf[idx].pos,ystart+num+1,
                     dbuf[idx].str,dl_setting.keys_attr,dl_setting.short_attr)
          else
            ShowCStr(screen_ptr,dbuf[idx].pos,ystart+num+1,
                     dbuf[idx].str,dl_setting.disbld_attr,dl_setting.disbld_attr);
        end;

      If (spos < 1) then spos := 1;
      If (spos > nm2) then spos := nm2;

      idx2 := spos;
      idx3 := 1;

      If NOT dbuf[idx2].use then
        begin
          SubPos(idx2);
          If NOT dbuf[idx2].use then AddPos(idx2);
        end;

      ShowItem;
      ShowItem;
      qflg := FALSE;
      If (keys = '$') then
        begin
          _dbg_leave; EXIT; //Dialog
        end;

      Repeat
        dl_environment.cur_item := idx2;
        If (Addr(dl_environment.ext_proc) <> NIL) then dl_environment.ext_proc;
        key := GetKey;
        If LookUpKey(key,dl_setting.terminate_keys,50) then qflg := TRUE;

        If NOT qflg then
          If (LO(key) in [$20..$0ff]) then
            begin
              idx := RetKey(LO(key));
              If (idx <> 0) then
                begin
                  qflg := TRUE;
                  idx2 := idx;
                end;
            end
          else If NOT shift_pressed and
                  NOT ctrl_pressed and NOT alt_pressed then
                 Case key of
                   kLEFT: If (idx2 > 1) or
                             NOT dl_setting.cycle_moves then SubPos(idx2)
                          else begin
                                 idx2 := nm2;
                                 If NOT dbuf[idx2].use then SubPos(idx2);
                               end;

                   kRIGHT: If (idx2 < nm2) or
                              NOT dl_setting.cycle_moves then
                             begin
                               temp := idx2;
                               AddPos(idx2);
                               If (idx2 = temp) then
                                 begin
                                   idx2 := 1;
                                   If NOT dbuf[idx2].use then AddPos(idx2);
                                 end;
                             end
                           else begin
                                  idx2 := 1;
                                  If NOT dbuf[idx2].use then AddPos(idx2);
                                end;

                   kHOME: begin
                            idx2 := 1;
                            If NOT dbuf[idx2].use then AddPos(idx2);
                          end;

                   kEND: begin
                           idx2 := nm2;
                           If NOT dbuf[idx2].use then SubPos(idx2);
                         end;
                 end;

        ShowItem;
{$IFNDEF GO32V2}
        draw_screen;
{$ENDIF}
      until qflg or _force_program_quit;

      Dialog := idx2;
      dl_environment.keystroke := key;
    end;

  If Addr(move_to_screen_routine) <> NIL then
    begin
      move_to_screen_data := ptr_scr_backup;
      move_to_screen_area[1] := xstart;
      move_to_screen_area[2] := ystart;
      move_to_screen_area[3] := xstart+max+3+2;
      move_to_screen_area[4] := ystart+num+2+1;
      move_to_screen_routine;
    end
  else
    ScreenMemCopy(ptr_scr_backup,screen_ptr);

  _dbg_leave; //EXIT //Dialog
end;
