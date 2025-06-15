// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function Menu(var data; x,y: Byte; spos: Word;
              len,len2: Byte; count: Word; title: String): Word;

{$I Menu_SubPos.pas}
{$I Menu_AddPos.pas}
{$I Menu_RetKey.pas}
{$I Menu_edit_contents.pas}

begin { Menu }
  _dbg_enter ({$I %FILE%}, 'Menu');

  If (count = 0) then
    begin
      Menu := 0;
      _dbg_leave; EXIT; //Menu
    end;

  max := Length(title);
  mnu_data := Addr(data);
  mnu_count := count;
  mnu_len := len;

  If NOT mn_environment.unpolite then
    ScreenMemCopy(mn_environment.v_dest,ptr_scr_backup2);

  If (count < 1) then
    begin
      _dbg_leave; EXIT; //Menu
    end;
  vscrollbar_pos := WORD_NULL;

  If NOT mn_environment.preview then HideCursor;
  temp := 0;

  For idx := 1 to count do
    begin
      mbuf[idx].key := OutKey(pstr(idx));
      If NOT mn_setting.reverse_use then mbuf[idx].use := mbuf[idx].key <> '~'
      else mbuf[idx].use := NOT (mbuf[idx].key <> '~');
      If mbuf[idx].use then temp := 1;
    end;

  solid := FALSE;
  If (temp = 0) then
    begin
      For temp := 1 to count do mbuf[temp].use := TRUE;
      solid := TRUE;
    end;

  For idx := 1 to count do
    If (max < CStrLen(pstr(idx))+mn_environment.descr_len) then
      max := CStrLen(pstr(idx))+mn_environment.descr_len;

  If mn_setting.center_box then
    begin
      x := (work_MaxCol-max-2) DIV 2+(work_MaxCol-max-2) MOD 2;
      y := (work_MaxLn-len2-1) DIV 2+(work_MaxLn-len2-1) MOD 2;
    end;

  mnu_x := x;
  mnu_y := y;
  len2b := len2;
  mn_environment.xpos := x;
  mn_environment.ypos := y;
  mn_environment.xsize := max+1;
  mn_environment.ysize := len2+1;
  mn_environment.desc_pos := y+len2+1;

  If NOT mn_environment.unpolite then
    begin
      old_fr_shadow_enabled := fr_setting.shadow_enabled;
      fr_setting.shadow_enabled := mn_setting.shadow_enabled;
      If mn_environment.intact_area then
        fr_setting.update_area := FALSE;
      If mn_setting.frame_enabled then
        Frame(mn_environment.v_dest,x,y,x+max+1,y+len2+1,mn_setting.menu_attr,
              title,mn_setting.title_attr,mn_setting.frame_type);
      If mn_environment.intact_area then
        fr_setting.update_area := TRUE;
      fr_setting.shadow_enabled := old_fr_shadow_enabled;

      contxt := DietStr(mn_environment.context,max+
        (Length(mn_environment.context)-CStrLen(mn_environment.context)));
      If mn_setting.frame_enabled then
        ShowC3Str(mn_environment.v_dest,x+1,y+len2+1,
                  '`'+ExpStrL('',max-CStrLen(contxt),
                              mn_setting.frame_type[2])+'`'+
                  contxt,
                  mn_setting.contxt_attr,
                  mn_setting.contxt2_attr,
                  mn_setting.menu_attr);

      temp2 := len2;
      mnu_len2 := len2;

      If (len2 > count) then len2 := count;
      If (len2 < 1) then len2 := 1;
      If (spos < 1) then spos := 1;
      If (spos > count) then spos := count;

      mn_environment.refresh := refresh;

      first := 1;
      last := count;
      While NOT mbuf[first].use do Inc(first);
      While NOT mbuf[last].use do Dec(last);

      If (first <= mn_setting.topic_len) then
        first := SUCC(mn_setting.topic_len);
      If (spos < first) or (spos > last) then
        spos := first;
      idx2 := 1;
      page := 1;
      opage := WORD_NULL;
      opos := WORD_NULL;
      While (idx2+page-1 < spos) do AddPos(idx2);
    end;

  mnu_topic_len := mn_setting.topic_len;
  If (mnu_topic_len <> 0) then
    begin
      mn_setting.topic_len := 0;
      refresh;

      mn_setting.topic_len := mnu_topic_len;
      mnu_topic_len := 0;
      mnu_data := mnu_data+SUCC(len)*mn_setting.topic_len;
      Inc(mnu_y,mn_setting.topic_len);
      Dec(len2,mn_setting.topic_len);
      Dec(mnu_len2,mn_setting.topic_len);
      Move(mbuf[SUCC(mn_setting.topic_len)],mbuf[1],
              (count-mn_setting.topic_len)*SizeOf(mbuf[1]));

      For temp := 1 to mn_setting.topic_len do SubPos(idx2);
      Dec(count,mn_setting.topic_len);
      Dec(mnu_count,mn_setting.topic_len);
      Dec(first,mn_setting.topic_len);
      Dec(last,mn_setting.topic_len);
      refresh;
    end
  else
    refresh;

  mn_environment.curr_page := page;
  mn_environment.curr_pos := idx2+page-1;
  mn_environment.curr_item := CutStr(pstr(idx2+page-1));
  mn_environment.keystroke := WORD_NULL;
  If (Addr(mn_environment.ext_proc) <> NIL) then mn_environment.ext_proc;

  qflg := FALSE;
  If mn_environment.preview then
    begin
      mn_environment.preview  := FALSE;
      mn_environment.unpolite := TRUE;
    end
  else
    begin
      Repeat
        mn_environment.keystroke := key;
        key := GetKey;
        If NOT qflg then
          If (LO(key) in [$20..$0ff]) then
            begin
              idx := RetKey(LO(key));
              If (idx <> 0) then
                begin
                  refresh;
                  idx2 := idx;
                  If NOT ((key = mn_setting.terminate_keys[2]) and
                           mn_setting.edit_contents) then qflg := TRUE
                  else edit_contents(idx);
                end;
            end
          else If NOT shift_pressed and
                  NOT ctrl_pressed and NOT alt_pressed then
                 Case key of
                   kUP: If (page+idx2-1 > first) or
                          NOT mn_setting.cycle_moves then SubPos(idx2)
                        else begin
                               idx2 := len2;
                               page := count-len2+1;
                               If NOT mbuf[idx2+page-1].use then SubPos(idx2);
                             end;
                   kDOWN: If (page+idx2-1 < last) or
                            NOT mn_setting.cycle_moves then AddPos(idx2)
                          else begin
                                 idx2 := 1;
                                 page := 1;
                                 If NOT mbuf[idx2+page-1].use then AddPos(idx2);
                               end;
                   kHOME: begin
                            If (mn_setting.homing_pos = 0) then begin idx2 := 1; page := 1; end
                            else If (idx2+page-1 > mn_setting.homing_pos) and
                                    (mn_setting.homing_pos < count) then
                                   Repeat SubPos(idx2) until (idx2+page-1 <= mn_setting.homing_pos)
                                 else begin
                                        idx2 := 1;
                                        page := 1;
                                      end;
                            If NOT mbuf[idx2+page-1].use then AddPos(idx2);
                          end;

                   kEND: begin
                           If (mn_setting.homing_pos = 0) then begin idx2 := len2; page := count-len2+1; end
                           else If (idx2+page-1 < mn_setting.homing_pos) and
                                   (mn_setting.homing_pos < count) then
                                  Repeat
                                    AddPos(idx2);
                                  until (idx2+page-1 >= mn_setting.homing_pos)
                                else begin
                                       idx2 := len2;
                                       page := count-len2+1;
                                     end;
                           If NOT mbuf[idx2+page-1].use then SubPos(idx2);
                         end;

                   kPgUP: If (idx2+page-1-(len2-1) > mn_setting.homing_pos) or
                             (idx2+page-1 <= mn_setting.homing_pos) or
                             (mn_setting.homing_pos = 0) or
                             NOT (mn_setting.homing_pos < count) then
                            For temp := 1 to len2-1 do SubPos(idx2)
                          else Repeat
                                 SubPos(idx2);
                               until (idx2+page-1 <= mn_setting.homing_pos);

                   kPgDOWN: If (idx2+page-1+(len2-1) < mn_setting.homing_pos) or
                               (idx2+page-1 >= mn_setting.homing_pos) or
                               (mn_setting.homing_pos = 0) or
                               NOT (mn_setting.homing_pos < count) then
                              For temp := 1 to len2-1 do AddPos(idx2)
                            else Repeat
                                   AddPos(idx2);
                                 until (idx2+page-1 >= mn_setting.homing_pos);
                 end;

        If LookUpKey(key,mn_setting.terminate_keys,50) then
          If NOT ((key = mn_setting.terminate_keys[2]) and
                   mn_setting.edit_contents) then qflg := TRUE
          else edit_contents(idx2+page-1);

        mn_environment.curr_page := page;
        mn_environment.curr_pos := idx2+page-1;
        mn_environment.curr_item := CutStr(pstr(idx2+page-1));
        refresh;
        mn_environment.keystroke := key;
        If (Addr(mn_environment.ext_proc) <> NIL) then mn_environment.ext_proc;
{$IFNDEF GO32V2}
        draw_screen;
{$ENDIF}
      until qflg or _force_program_quit;
    end;

  If mn_environment.winshade and NOT mn_environment.unpolite then
    begin
      If (Addr(move_to_screen_routine) <> NIL) then
        begin
          move_to_screen_data := ptr_scr_backup2;
          move_to_screen_area[1] := x;
          move_to_screen_area[2] := y;
          move_to_screen_area[3] := x+max+1+2;
          move_to_screen_area[4] := y+len2b+1+1;
          move_to_screen_routine;
         end
      else
        ScreenMemCopy(ptr_scr_backup2,mn_environment.v_dest);
    end;

  Menu := idx2+page-1;

  _dbg_leave; //EXIT //Menu
end;
