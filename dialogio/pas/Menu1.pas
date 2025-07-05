// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// From `MenuLib1' unit

function MenuLib1_Menu(var data; x,y: Byte; spos: Word;
                       len,len2: Byte; count: Word; title: String): Word;

{$I dialogio/pas/Menu1_SubPos.pas}
{$I dialogio/pas/Menu1_AddPos.pas}
{$I dialogio/pas/Menu1_RetKey.pas}
{$I dialogio/pas/Menu1_edit_contents.pas}

begin { MenuLib1_Menu }
  _dbg_enter ({$I %FILE%}, 'MenuLib1_Menu');

  If (count = 0) then
    begin
      MenuLib1_Menu := 0;
      _dbg_leave; EXIT; //MenuLib1_Menu
    end;

  max := Length(title);
  mnu_data := Addr(data);
  mnu_count := count;
  mnu_len := len;

  If NOT MenuLib1_mn_environment.unpolite then
    ScreenMemCopy(MenuLib1_mn_environment.v_dest,ptr_scr_backup);

  If (count < 1) then
    begin
      _dbg_leave; EXIT; //MenuLib1_Menu
    end;
  vscrollbar_pos := BYTE_NULL;

  If NOT MenuLib1_mn_environment.preview then HideCursor;
  temp := 0;

  For idx := 1 to count do
    begin
      mbuf[idx].key := OutKey(pstr(idx));
      If NOT MenuLib1_mn_setting.reverse_use then mbuf[idx].use := mbuf[idx].key <> '~'
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
    If (max < CStrLen(pstr(idx))+MenuLib1_mn_environment.descr_len) then
      max := CStrLen(pstr(idx))+MenuLib1_mn_environment.descr_len;

  If MenuLib1_mn_setting.center_box then
    begin
      x := (work_MaxCol-max-2) DIV 2+(work_MaxCol-max-2) MOD 2;
      y := (work_MaxLn-len2-1) DIV 2+(work_MaxLn-len2-1) MOD 2;
    end;

  mnu_x := x;
  mnu_y := y;
  len2b := len2;

  If NOT MenuLib1_mn_environment.unpolite then
    begin
      old_fr_shadow_enabled := fr_setting.shadow_enabled;
      fr_setting.shadow_enabled := MenuLib1_mn_setting.shadow_enabled;
      fr_setting.update_area := FALSE;
      Frame(MenuLib1_mn_environment.v_dest,x,y,x+max+1,y+len2+1,MenuLib1_mn_setting.menu_attr,
            title,MenuLib1_mn_setting.title_attr,MenuLib1_mn_setting.frame_type);
      fr_setting.update_area := TRUE;
      fr_setting.shadow_enabled := old_fr_shadow_enabled;

      contxt := DietStr(MenuLib1_mn_environment.context,max+
        (Length(MenuLib1_mn_environment.context)-CStrLen(MenuLib1_mn_environment.context)));
      ShowCStr(MenuLib1_mn_environment.v_dest,x+max+1-CStrLen(contxt),y+len2+1,
               contxt,MenuLib1_mn_setting.contxt_attr,MenuLib1_mn_setting.contxt2_attr);

      temp2 := len2;
      If (len2 > count) then len2 := count;
      If (len2 < 1) then len2 := 1;
      If (spos < 1) then spos := 1;
      If (spos > count) then spos := count;

      mnu_len2 := len2;
      MenuLib1_mn_environment.refresh := refresh;

      first := 1;
      last := count;
      While NOT mbuf[first].use do Inc(first);
      While NOT mbuf[last].use do Dec(last);

      If (spos < first) or (spos > last) then
        spos := first;
      idx2 := 1;
      page := 1;
      opage := WORD_NULL;
      opos := WORD_NULL;
      While (idx2+page-1 < spos) do AddPos(idx2);
    end;

  MenuLib1_mn_environment.curr_pos := idx2+page-1;
  MenuLib1_mn_environment.keystroke := WORD_NULL;
  If (Addr(MenuLib1_mn_environment.ext_proc) <> NIL) then MenuLib1_mn_environment.ext_proc;

  qflg := FALSE;
  If MenuLib1_mn_environment.preview then
    begin
      MenuLib1_mn_environment.preview  := FALSE;
      MenuLib1_mn_environment.unpolite := TRUE;
      refresh;
    end
  else
    Repeat
      key := GetKey;
      If LookUpKey(key,MenuLib1_mn_setting.terminate_keys,50) then
        If NOT ((key = MenuLib1_mn_setting.terminate_keys[2]) and
                 MenuLib1_mn_setting.edit_contents) then qflg := TRUE
        else edit_contents(idx2+page-1);

      If NOT qflg then
        If (LO(key) in [$20..$0ff]) then
          begin
            idx := RetKey(LO(key));
            If (idx <> 0) then
              begin
                refresh;
                idx2 := idx;
                If NOT ((key = MenuLib1_mn_setting.terminate_keys[2]) and
                         MenuLib1_mn_setting.edit_contents) then qflg := TRUE
                else edit_contents(idx);
              end;
          end
        else If NOT shift_pressed and
                NOT ctrl_pressed and NOT alt_pressed then
               Case key of
                 kUP: If (page+idx2-1 > first) or
                        NOT MenuLib1_mn_setting.cycle_moves then SubPos(idx2)
                      else begin
                             idx2 := len2;
                             page := count-len2+1;
                             If NOT mbuf[idx2+page-1].use then SubPos(idx2);
                           end;

                 kDOWN: If (page+idx2-1 < last) or
                           NOT MenuLib1_mn_setting.cycle_moves then AddPos(idx2)
                        else begin
                               idx2 := 1;
                               page := 1;
                               If NOT mbuf[idx2+page-1].use then AddPos(idx2);
                             end;

                 kHOME: begin
                          idx2 := 1;
                          page := 1;
                          If NOT mbuf[idx2+page-1].use then AddPos(idx2);
                        end;

                 kEND: begin
                         idx2 := len2;
                         page := count-len2+1;
                         If NOT mbuf[idx2+page-1].use then SubPos(idx2);
                       end;

                 kPgUP: For temp := 1 to len2-1 do SubPos(idx2);
                 kPgDOWN: For temp := 1 to len2-1 do AddPos(idx2);
               end;

      MenuLib1_mn_environment.curr_pos := idx2+page-1;
      refresh;
      MenuLib1_mn_environment.keystroke := key;
      If (Addr(MenuLib1_mn_environment.ext_proc) <> NIL) then MenuLib1_mn_environment.ext_proc;
{$IFDEF GO32V2}
      // draw_screen;
      keyboard_reset_buffer_alt;
{$ELSE}
      draw_screen;
      // keyboard_reset_buffer;
{$ENDIF}
    until qflg or _force_program_quit;

  If MenuLib1_mn_environment.winshade and NOT MenuLib1_mn_environment.unpolite then
    begin
      If (Addr(move_to_screen_routine) <> NIL) then
        begin
          move_to_screen_data := ptr_scr_backup;
          move_to_screen_area[1] := x;
          move_to_screen_area[2] := y;
          move_to_screen_area[3] := x+max+1+2;
          move_to_screen_area[4] := y+len2b+1+1;
          move_to_screen_routine;
         end
      else
        ScreenMemCopy(ptr_scr_backup,MenuLib1_mn_environment.v_dest);
    end;

  MenuLib1_Menu := idx2+page-1;

  _dbg_leave; //EXIT //MenuLib1_Menu
end;
