//  This file is part of Adlib Tracker II (AT2).
//
//  AT2 is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  AT2 is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with AT2.  If not, see <http://www.gnu.org/licenses/>.

unit MenuLib2;
{$S-,Q-,R-,V-,B-,X+}
{$PACKRECORDS 1}
interface

uses
  DOS,
  AdT2unit,
  AdT2sys,
  AdT2keyb,
  AdT2ext2,
  DialogIO,
  TxtScrIO,
  StringIO,
  ParserIO;

const
  MenuLib2_mn_setting: Record
                         frame_type:     String;
                         shadow_enabled: Boolean;
                         posbar_enabled: Boolean;
                         title_attr,
                         menu_attr,
                         text_attr,
                         text2_attr,
                         default_attr,
                         short_attr,
                         short2_attr,
                         disbld_attr,
                         contxt_attr,
                         contxt2_attr:   Byte;
                         center_box:     Boolean;
                         cycle_moves:    Boolean;
                         edit_contents:  Boolean;
                         reverse_use:    Boolean;
                         fixed_len:      Byte;
                         terminate_keys: array[1..50] of Word;
                       end

  = (frame_type:     frame_single;
     shadow_enabled: TRUE;
     posbar_enabled: TRUE;
     title_attr:     $0f;
     menu_attr:      $07;
     text_attr:      $07;
     text2_attr:     $70;
     default_attr:   $0f;
     short_attr:     $0f;
     short2_attr:    $70;
     disbld_attr:    $07;
     contxt_attr:    $0f;
     contxt2_attr:   $07;
     center_box:     TRUE;
     cycle_moves:    TRUE;
     edit_contents:  FALSE;
     reverse_use:    FALSE;
     fixed_len:      0;
     terminate_keys: ($011b,$1c0d,$0000,$0000,$0000,
                      $0000,$0000,$0000,$0000,$0000,
                      $0000,$0000,$0000,$0000,$0000,
                      $0000,$0000,$0000,$0000,$0000,
                      $0000,$0000,$0000,$0000,$0000,
                      $0000,$0000,$0000,$0000,$0000,
                      $0000,$0000,$0000,$0000,$0000,
                      $0000,$0000,$0000,$0000,$0000,
                      $0000,$0000,$0000,$0000,$0000,
                      $0000,$0000,$0000,$0000,$0000));
var
  MenuLib2_mn_environment: Record
                             v_dest:     tSCREEN_MEM_PTR;
                             keystroke:  Word;
                             context:    String;
                             unpolite:   Boolean;
                             winshade:   Boolean;
                             edit_pos:   Byte;
                             curr_pos:   Word;
                             ext_proc:   procedure;
                             refresh:    procedure;
                             do_refresh: Boolean;
                             preview:    Boolean;
                             descr_len:  Byte;
                             descr:      Pointer;
                           end;

function MenuLib2_Menu(var data; x,y: Byte; spos: Word;
                       len,len2: Byte; count: Word; title: String): Word;
procedure MenuLib2_Init;

implementation

uses
  debug;

type
  tDBUFFR = array[1.. 100] of Record
                                str: String;
                                pos: Byte;
                                key: Char;
                                use: Boolean;
                              end;
type
  tMBUFFR = array[1..16384] of Record
                                 key: Char;
                                 use: Boolean;
                               end;
var
  idx,idx2,max,len2b,page,
  first,last,temp,temp2,opage,opos: Word;
  old_fr_shadow_enabled: Boolean;
  key:    Word;
  solid:  Boolean;
  qflg:   Boolean;
  mbuf:   tMBUFFR;
  contxt: String;

var
  mnu_x,mnu_y,mnu_len,mnu_len2: Byte;
  mnu_data: Pointer;
  mnu_count: Word;

var
  vscrollbar_pos: Word;

{$I dialogio/pas/OutKey.pas}

function pstr(item: Word): String;

var
  temp: String;

begin
  _dbg_enter ({$I %FILE%}, 'pstr');

  Move(pBYTE(mnu_data)[(item-1)*(mnu_len+1)],temp,mnu_len+1);
  If NOT solid then pstr := ExpStrR(temp,mnu_len-2,' ')
  else pstr := ExpStrR(temp,mnu_len,' ');

  _dbg_leave; //EXIT //pstr
end;

function pdes(item: Word): String;

var
  temp: String;

begin
  _dbg_enter ({$I %FILE%}, 'pdes');

  If (mn_environment.descr <> NIL) then
    Move(pBYTE(mn_environment.descr)[(item-1)*(mn_environment.descr_len+1)],temp,mn_environment.descr_len+1)
  else temp := '';
  pdes := ExpStrR(temp,mn_environment.descr_len,' ');

  _dbg_leave; //EXIT //pdes
end;

procedure refresh;

procedure ShowCStr_clone(dest: tSCREEN_MEM_PTR; x,y: Byte; str: String;
                         atr1,atr2,atr3,atr4: Byte);
var
  temp,len: Byte;
  highlighted: Boolean;

begin
  _dbg_enter ({$I %FILE%}, 'refresh.ShowCStr_clone');

  If NOT (MenuLib2_mn_setting.fixed_len <> 0) then
    begin
      ShowCStr(dest,x,y,str,atr1,atr2);
      _dbg_leave; EXIT; //refresh.ShowCStr_clone
    end;

  highlighted := FALSE;
  len := 0;
  For temp := 1 to Length(str) do
    If (str[temp] = '~') then highlighted := NOT highlighted
    else begin
           If (len < MenuLib2_mn_setting.fixed_len) then
             begin
               If NOT highlighted then ShowStr(dest,x+len,y,str[temp],atr1)
               else ShowStr(dest,x+len,y,str[temp],atr2);
               Inc(len);
             end
           else
             begin
               If NOT highlighted then ShowStr(dest,x+len,y,str[temp],atr3)
               else ShowStr(dest,x+len,y,str[temp],atr4);
               Inc(len);
             end
         end;

  _dbg_leave; //EXIT //refresh.ShowCStr_clone
end;

begin
  _dbg_enter ({$I %FILE%}, 'refresh');

  If (page = opage) and (idx2 = opos) and NOT MenuLib2_mn_environment.do_refresh then
    begin
      _dbg_leave; EXIT; //refresh
    end
  else begin
         opage := page;
         opos  := idx2;
         MenuLib2_mn_environment.do_refresh := FALSE;
       end;

  For idx := page to mnu_len2+page-1 do
    If (idx = idx2+page-1) then
      ShowCStr_clone(MenuLib2_mn_environment.v_dest,mnu_x+1,mnu_y+idx2,
                     ExpStrR(pstr(idx2+page-1)+pdes(idx2+page-1),
                     max+(Length(pstr(idx2+page-1))+Length(pdes(idx2+page-1))-
                     CStrLen(pstr(idx2+page-1)+pdes(idx2+page-1))),' '),
                     MenuLib2_mn_setting.text2_attr,MenuLib2_mn_setting.short2_attr,
                     MenuLib2_mn_setting.text_attr,MenuLib2_mn_setting.short_attr)
    else
      If mbuf[idx].use then
        ShowCStr(MenuLib2_mn_environment.v_dest,mnu_x+1,mnu_y+idx-page+1,
                 ExpStrR(pstr(idx)+pdes(idx),
                 max+(Length(pstr(idx))+Length(pdes(idx2+page-1))-
                 CStrLen(pstr(idx)+pdes(idx))),' '),
                 MenuLib2_mn_setting.text_attr,MenuLib2_mn_setting.short_attr)
      else
        ShowCStr(MenuLib2_mn_environment.v_dest,mnu_x+1,mnu_y+idx-page+1,
                 ExpStrR(pstr(idx)+pdes(idx),
                 max+(Length(pstr(idx))+Length(pdes(idx2+page-1))-
                 CStrLen(pstr(idx)+pdes(idx))),' '),
                 MenuLib2_mn_setting.disbld_attr,MenuLib2_mn_setting.disbld_attr);

  vscrollbar_pos :=
    VScrollBar(MenuLib2_mn_environment.v_dest,mnu_x+max+1,mnu_y+1,temp2,mnu_count,idx2+page-1,
               vscrollbar_pos,MenuLib2_mn_setting.menu_attr,MenuLib2_mn_setting.menu_attr);

  _dbg_leave; //EXIT //refresh
end;

function MenuLib2_Menu(var data; x,y: Byte; spos: Word;
                       len,len2: Byte; count: Word; title: String): Word;

procedure SubPos(var p: Word);

var
  temp: Word;

begin
  temp := p;
  If (temp > 1) and mbuf[temp+page-2].use then
    Dec(temp)
  else If (temp > 1) then
         begin
           Dec(temp);
           SubPos(temp);
         end
       else If (page > first) then
              Dec(page);
  If mbuf[temp+page-1].use then p := temp
  else If (temp+page-1 > first) then SubPos(temp);
end;

procedure AddPos(var p: Word);

var
  temp: Word;

begin
  temp := p;
  If (temp < len2) and (temp < last) and
     mbuf[temp+page].use then
    Inc(temp)
  else If (temp < len2) and (temp < last) then
         begin
           Inc(temp);
           AddPos(temp);
         end
       else If (page+temp <= last) then
              Inc(page);
  If mbuf[temp+page-1].use then p := temp
  else If (temp+page-1 < last) then AddPos(temp);
end;

function RetKey(code: Byte): Word;

var
  temp: Byte;

begin
  RetKey := 0;
  For temp := 1 to count do
    If (UpCase(mbuf[temp].key) = UpCase(CHR(code))) then
      begin
        RetKey := temp;
        BREAK;
      end;
end;

procedure edit_contents(item: Word);

var
  temp: String;

begin
  _dbg_enter ({$I %FILE%}, 'MenuLib2_Menu.edit_contents');

  is_setting.append_enabled := TRUE;
  is_setting.character_set  := [#32..#255];
  is_environment.locate_pos := 1;

  If (MenuLib2_mn_environment.edit_pos > 0) and (MenuLib2_mn_environment.edit_pos < max-2) then
    temp := Copy(pstr(item),MenuLib2_mn_environment.edit_pos+1,
                      Length(pstr(item))-MenuLib2_mn_environment.edit_pos+1)
  else
    temp := CutStr(pstr(item));

  While (temp <> '') and (temp[Length(temp)] = ' ') do Delete(temp,Length(temp),1);
  temp := InputStr(temp,x+1+MenuLib2_mn_environment.edit_pos,y+idx2,
               max-2-MenuLib2_mn_environment.edit_pos+1,
               max-2-MenuLib2_mn_environment.edit_pos+1,
               MenuLib2_mn_setting.text2_attr,MenuLib2_mn_setting.default_attr);

  If (is_environment.keystroke = kENTER) then
    begin
      If (MenuLib2_mn_environment.edit_pos > 0) and (MenuLib2_mn_environment.edit_pos < max-2) then
        temp := Copy(pstr(item),1,MenuLib2_mn_environment.edit_pos)+temp
      else
        temp := CutStr(temp);
      Move(temp,pBYTE(mnu_data)[(item-1)*(len+1)],len+1);
    end;

  ShowCStr(MenuLib2_mn_environment.v_dest,x+1,y+idx2,
           ExpStrR(pstr(item),max+(Length(pstr(item)))-
           CStrLen(pstr(item)),' '),
           MenuLib2_mn_setting.text2_attr,MenuLib2_mn_setting.short2_attr);

  _dbg_leave; //EXIT //MenuLib2_Menu.edit_contents
end;

begin { MenuLib2_Menu }
  _dbg_enter ({$I %FILE%}, 'MenuLib2_Menu');

  If (count = 0) then
    begin
      MenuLib2_Menu := 0;
      _dbg_leave; EXIT; //MenuLib2_Menu
    end;

  max := Length(title);
  mnu_data := Addr(data);
  mnu_count := count;
  mnu_len := len;

  If NOT MenuLib2_mn_environment.unpolite then
    ScreenMemCopy(MenuLib2_mn_environment.v_dest,ptr_scr_backup);

  If (count < 1) then
    begin
      _dbg_leave; EXIT; //MenuLib2_Menu
    end;
  vscrollbar_pos := BYTE_NULL;

  If NOT MenuLib2_mn_environment.preview then HideCursor;
  temp := 0;

  For idx := 1 to count do
    begin
      mbuf[idx].key := OutKey(pstr(idx));
      If NOT MenuLib2_mn_setting.reverse_use then mbuf[idx].use := mbuf[idx].key <> '~'
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
    If (max < CStrLen(pstr(idx))+MenuLib2_mn_environment.descr_len) then
      max := CStrLen(pstr(idx))+MenuLib2_mn_environment.descr_len;

  If MenuLib2_mn_setting.center_box then
    begin
      x := (work_MaxCol-max-2) DIV 2+(work_MaxCol-max-2) MOD 2;
      y := (work_MaxLn-len2-1) DIV 2+(work_MaxLn-len2-1) MOD 2;
    end;

  mnu_x := x;
  mnu_y := y;
  len2b := len2;

  If NOT MenuLib2_mn_environment.unpolite then
    begin
      old_fr_shadow_enabled := fr_setting.shadow_enabled;
      fr_setting.shadow_enabled := MenuLib2_mn_setting.shadow_enabled;
      fr_setting.update_area := FALSE;
      Frame(MenuLib2_mn_environment.v_dest,x,y,x+max+1,y+len2+1,MenuLib2_mn_setting.menu_attr,
            title,MenuLib2_mn_setting.title_attr,MenuLib2_mn_setting.frame_type);
      fr_setting.update_area := TRUE;
      fr_setting.shadow_enabled := old_fr_shadow_enabled;

      contxt := DietStr(MenuLib2_mn_environment.context,max+
        (Length(MenuLib2_mn_environment.context)-CStrLen(MenuLib2_mn_environment.context)));
      ShowCStr(MenuLib2_mn_environment.v_dest,x+max+1-CStrLen(contxt),y+len2+1,
               contxt,MenuLib2_mn_setting.contxt_attr,MenuLib2_mn_setting.contxt2_attr);

      temp2 := len2;
      If (len2 > count) then len2 := count;
      If (len2 < 1) then len2 := 1;
      If (spos < 1) then spos := 1;
      If (spos > count) then spos := count;

      mnu_len2 := len2;
      MenuLib2_mn_environment.refresh := refresh;

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

  MenuLib2_mn_environment.curr_pos := idx2+page-1;
  MenuLib2_mn_environment.keystroke := WORD_NULL;
  If (Addr(MenuLib2_mn_environment.ext_proc) <> NIL) then MenuLib2_mn_environment.ext_proc;

  qflg := FALSE;
  If MenuLib2_mn_environment.preview then
    begin
      MenuLib2_mn_environment.preview  := FALSE;
      MenuLib2_mn_environment.unpolite := TRUE;
      refresh;
    end
  else
    Repeat
      key := GetKey;
      If LookUpKey(key,MenuLib2_mn_setting.terminate_keys,50) then
        If NOT ((key = MenuLib2_mn_setting.terminate_keys[2]) and
                 MenuLib2_mn_setting.edit_contents) then qflg := TRUE
        else edit_contents(idx2+page-1);

      If NOT qflg then
        If (LO(key) in [$20..$0ff]) then
          begin
            idx := RetKey(LO(key));
            If (idx <> 0) then
              begin
                refresh;
                idx2 := idx;
                If NOT ((key = MenuLib2_mn_setting.terminate_keys[2]) and
                         MenuLib2_mn_setting.edit_contents) then qflg := TRUE
                else edit_contents(idx);
              end;
          end
        else If NOT shift_pressed and
                NOT ctrl_pressed and NOT alt_pressed then
               Case key of
                 kUP: If (page+idx2-1 > first) or
                        NOT MenuLib2_mn_setting.cycle_moves then SubPos(idx2)
                      else begin
                             idx2 := len2;
                             page := count-len2+1;
                             If NOT mbuf[idx2+page-1].use then SubPos(idx2);
                           end;

                 kDOWN: If (page+idx2-1 < last) or
                           NOT MenuLib2_mn_setting.cycle_moves then AddPos(idx2)
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

      MenuLib2_mn_environment.curr_pos := idx2+page-1;
      refresh;
      MenuLib2_mn_environment.keystroke := key;
      If (Addr(MenuLib2_mn_environment.ext_proc) <> NIL) then MenuLib2_mn_environment.ext_proc;
{$IFDEF GO32V2}
      // draw_screen;
      keyboard_reset_buffer_alt;
{$ELSE}
      draw_screen;
      // keyboard_reset_buffer;
{$ENDIF}
    until qflg or _force_program_quit;

  If MenuLib2_mn_environment.winshade and NOT MenuLib2_mn_environment.unpolite then
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
        ScreenMemCopy(ptr_scr_backup,MenuLib2_mn_environment.v_dest);
    end;

  MenuLib2_Menu := idx2+page-1;

  _dbg_leave; //EXIT //MenuLib2_Menu
end;

procedure MenuLib2_Init;
begin
  _dbg_enter ({$I %FILE%}, 'MenuLib2_Init');

  MenuLib2_mn_setting.frame_type     := frame_single;
  MenuLib2_mn_setting.center_box     := FALSE;
  MenuLib2_mn_setting.shadow_enabled := FALSE;
  MenuLib2_mn_setting.cycle_moves    := FALSE;
  MenuLib2_mn_setting.title_attr     := dialog_background+dialog_title;
  MenuLib2_mn_setting.menu_attr      := dialog_background+dialog_text;
  MenuLib2_mn_setting.text_attr      := dialog_background+dialog_item;
  MenuLib2_mn_setting.text2_attr     := dialog_sel_itm_bck+dialog_sel_itm;
  MenuLib2_mn_setting.default_attr   := dialog_def_bckg+dialog_def;
  MenuLib2_mn_setting.short_attr     := dialog_background+dialog_short;
  MenuLib2_mn_setting.short2_attr    := dialog_sel_itm_bck+dialog_sel_short;
  MenuLib2_mn_setting.disbld_attr    := dialog_background+dialog_item_dis;
  MenuLib2_mn_setting.contxt_attr    := dialog_background+dialog_context;
  MenuLib2_mn_setting.contxt2_attr   := dialog_background+dialog_context_dis;
  MenuLib2_mn_setting.terminate_keys[3] := kTAB;
  MenuLib2_mn_setting.terminate_keys[4] := kLEFT;
  MenuLib2_mn_setting.terminate_keys[5] := kRIGHT;
  MenuLib2_mn_setting.terminate_keys[6] := kCtrlO;
  MenuLib2_mn_setting.terminate_keys[7] := kF1;
  MenuLib2_mn_setting.terminate_keys[8] := kShTAB;

  MenuLib2_mn_environment.v_dest     := screen_ptr;
  MenuLib2_mn_environment.keystroke  := $0000;
  MenuLib2_mn_environment.context    := '';
  MenuLib2_mn_environment.unpolite   := FALSE;
  MenuLib2_mn_environment.winshade   := TRUE;
  MenuLib2_mn_environment.ext_proc   := NIL;
  MenuLib2_mn_environment.refresh    := NIL;
  MenuLib2_mn_environment.do_refresh := FALSE;
  MenuLib2_mn_environment.preview    := FALSE;
  MenuLib2_mn_environment.descr_len  := 0;
  MenuLib2_mn_environment.descr      := NIL;

  _dbg_leave; //EXIT //MenuLib2_Init
end;

end.
