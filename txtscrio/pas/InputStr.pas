// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function InputStr(s: String; x,y,ln,ln1: Byte; atr1,atr2: Byte): String;

var
  appn,for1st,qflg,chflag,ins: Boolean;
  cloc,xloc,xint,attr: Byte;
  key: Word;
  s1,s2: String;

function more(value1,value2: Byte): Byte;
begin
  If (value1 >= value2) then more := value1
  else more := value2;
end;

label _end;

begin { InputStr }
  _dbg_enter ({$I %FILE%}, 'InputStr');

  s := Copy(s,1,ln);
  If (is_environment.locate_pos > ln1) then
    is_environment.locate_pos := ln1;
  If (is_environment.locate_pos > Length(s)+1) then
    is_environment.locate_pos := Length(s);

  cloc := is_environment.locate_pos;
  xloc := is_environment.locate_pos;
  xint := x;
  qflg := FALSE;
  ins  := is_setting.insert_mode;
  appn := NOT is_setting.append_enabled;

  Dec(x);
  If ins then
    If use_large_cursor then WideCursor
    else ThinCursor
  else
    If use_large_cursor then ThinCursor
    else WideCursor;

  s1 := s;
  If (BYTE(s1[0]) > ln1) then s1[0] := CHR(ln1);

  ShowStr(screen_ptr,xint,y,ExpStrR('',ln1,' '),atr1);
  ShowStr(screen_ptr,xint,y,FilterStr2(s1,is_setting.char_filter,'_'),atr2);
  for1st := TRUE;

  Repeat
    s2 := s1;
    If (xloc = 1) then s1 := Copy(s,cloc,ln1)
    else s1 := Copy(s,cloc-xloc+1,ln1);

    If NOT appn then attr := atr2
    else attr := atr1;

    If appn and for1st then
      begin
        ShowStr(screen_ptr,xint,y,ExpStrR(FilterStr2(s1,is_setting.char_filter,'_'),ln1,' '),atr1);
        for1st := FALSE;
      end;

    If (s2 <> s1) then
      ShowStr(screen_ptr,xint,y,ExpStrR(FilterStr2(s1,is_setting.char_filter,'_'),ln1,' '),atr1);

    If (ln1 < ln) then
      If (cloc-xloc > 0) and (Length(s) > 0) then
        ShowStr(screen_ptr,xint,y,#17,(attr AND $0f0)+$0f)
      else If (cloc-xloc = 0) and (Length(s) <> 0) then
             ShowStr(screen_ptr,xint,y,s[1],attr)
           else
             ShowStr(screen_ptr,xint,y,' ',atr1);

    If (ln1 < ln) then
      If (cloc-xloc+ln1 < Length(s)) then
        ShowStr(screen_ptr,xint+ln1-1,y,#16,(attr AND $0f0)+$0f)
      else If (cloc-xloc+ln1 = Length(s)) then
             ShowStr(screen_ptr,xint+ln1-1,y,FilterStr2(s[Length(s)],is_setting.char_filter,'_'),attr)
           else
             ShowStr(screen_ptr,xint+ln1-1,y,' ',atr1);

    GotoXY(x+xloc,y);
    If keypressed then key := GetKey else GOTO _end;
    If LookupKey(key,is_setting.terminate_keys,50) then qflg := TRUE;
    chflag := FALSE;

    If NOT qflg then
      Case key of
        kTAB: appn := TRUE;

        kCHplus,
        kNPplus: begin
                   chflag := TRUE;
                   If (is_setting.character_set = DEC_NUM_CHARSET) then
                     If (Length(Num2str(SUCC(Str2num(s,10)),10)) <= ln1) and
                        (SUCC(Str2num(s,10)) <= is_environment.max_num) then
                       begin
                         s := Num2str(SUCC(Str2num(s,10)),10);
                         chflag := FALSE;
                       end
                     else
                   else If (is_setting.character_set = HEX_NUM_CHARSET) then
                          If (Length(Num2str(SUCC(Str2num(s,16)),16)) <= ln1) and
                             (SUCC(Str2num(s,16)) <= is_environment.max_num) then
                            begin
                              s := Num2str(SUCC(Str2num(s,16)),16);
                              chflag := FALSE;
                            end;
                 end;
        kCHmins,
        kNPmins: begin
                   chflag := TRUE;
                   If (is_setting.character_set = DEC_NUM_CHARSET) then
                     If (Str2num(s,10) > 0) and
                        (PRED(Str2num(s,10)) >= is_environment.min_num) then
                       begin
                         s := Num2str(PRED(Str2num(s,10)),10);
                         chflag := FALSE;
                       end
                     else
                   else If (is_setting.character_set = HEX_NUM_CHARSET) then
                          If (Str2num(s,16) > 0) and
                             (PRED(Str2num(s,16)) >= is_environment.min_num) then
                            begin
                              s := Num2str(PRED(Str2num(s,16)),16);
                              chflag := FALSE;
                            end;
                end;

        kCtrlY: begin
                  appn := TRUE;
                  s := '';
                  cloc := 1;
                  xloc := 1;
                end;

        kCtrlT: begin
                  appn := TRUE;
                  While (s[cloc] in is_setting.word_characters) and
                        (cloc <= Length(s)) do Delete(s,cloc,1);

                  While NOT (s[cloc] in is_setting.word_characters) and
                            (cloc <= Length(s)) do Delete(s,cloc,1);
                end;

        kCtrlK: begin
                  appn := TRUE;
                  Delete(s,cloc,Length(s));
                end;

        kCtBkSp: begin
                   appn := TRUE;
                   While (s[cloc-1] in is_setting.word_characters) and
                         (cloc > 1) do
                     begin
                       Dec(cloc); Delete(s,cloc,1);
                       If (xloc > 1) then Dec(xloc);
                     end;

                   While NOT (s[cloc-1] in is_setting.word_characters) and
                             (cloc > 1) do
                     begin
                       Dec(cloc); Delete(s,cloc,1);
                       If (xloc > 1) then Dec(xloc);
                     end;
                 end;

        kBkSPC: begin
                  appn := TRUE;
                  If (cloc > 1) then
                    begin
                      If (xloc > 1) then Dec(xloc);
                      Dec(cloc); Delete(s,cloc,1);
                    end;
                end;

        kDELETE: begin
                   appn := TRUE;
                   If (cloc <= Length(s)) then Delete(s,cloc,1);
                 end;

        kCtLEFT: begin
                   appn := TRUE;
                   While (s[cloc] in is_setting.word_characters) and
                         (cloc > 1) do
                     begin
                       Dec(cloc);
                       If (xloc > 1) then Dec(xloc);
                     end;

                   While NOT (s[cloc] in is_setting.word_characters) and
                             (cloc > 1) do
                     begin
                       Dec(cloc);
                       If (xloc > 1) then Dec(xloc);
                     end;
                 end;

        kCtRGHT: begin
                   appn := TRUE;
                   While (s[cloc] in is_setting.word_characters) and
                         (cloc < Length(s)) do
                     begin
                       Inc(cloc);
                       If (xloc < ln1) then Inc(xloc);
                     end;

                   While NOT (s[cloc] in is_setting.word_characters) and
                             (cloc < Length(s)) do
                     begin
                       Inc(cloc);
                       If (xloc < ln1) then Inc(xloc);
                     end;
                 end;

        kLEFT: begin
                 appn := TRUE;
                 If (cloc > 1) then Dec(cloc);
                 If (xloc > 1) then Dec(xloc);
               end;

        kRIGHT: begin
                  appn := TRUE;
                  If (cloc < Length(s)) or ((cloc = Length(s)) and
                       ((Length(s) < more(ln,ln1)))) then
                    Inc(cloc);
                  If (xloc < ln1) and (xloc <= Length(s)) then Inc(xloc);
                end;

        kINSERT: If is_setting.replace_enabled then
                   begin
                     ins := NOT ins;
                     If ins then
                       If use_large_cursor then WideCursor
                       else ThinCursor
                     else
                       If use_large_cursor then ThinCursor
                       else WideCursor;
                   end;

        kHOME: begin
                 appn := TRUE;
                 cloc := 1;
                 xloc := 1;
               end;

        kEND: begin
                appn := TRUE;
                If (Length(s) < more(ln,ln1)) then cloc := Succ(Length(s))
                else cloc := Length(s);
                If (cloc < ln1) then xloc := cloc else xloc := ln1;
              end;

        else chflag := TRUE;
      end;

    If chflag and (CHR(LO(key)) in tCHARSET(is_setting.character_set)) then
      begin
        If NOT appn then begin s := ''; cloc := 1; xloc := 1; end;
        appn := TRUE;
        If ins and (Length(CutStrR(s,cloc)) < ln) then
          begin
            If (Length(CutStrR(s,cloc)) < ln) then
              Insert(CHR(LO(key)),s,cloc)
            else s[cloc] := CHR(LO(key));
            s := FilterStr2(s,is_setting.valid_chars,'_');
            If (cloc < ln) then Inc(cloc);
            If (xloc < ln) and (xloc < ln1) then Inc(xloc)
          end
        else
          If (Length(s) < ln) or NOT ins then
            begin
              If (cloc > Length(s)) and (Length(s) < ln) then
                Inc(BYTE(s[0]));
              s[cloc] := CHR(LO(key));
              s := FilterStr2(s,is_setting.valid_chars,'_');
              If (cloc < ln) then Inc(cloc);
              If (xloc < ln) and (xloc < ln1) then Inc(xloc);
            end;
      end;
_end:
      is_environment.cur_str := s;
      If (Addr(is_environment.ext_proc) <> NIL) then is_environment.ext_proc;
{$IFDEF GO32V2}
      // draw_screen;
      keyboard_reset_buffer_alt;
{$ELSE}
      draw_screen;
      // keyboard_reset_buffer;
{$ENDIF}
  until qflg;

  If (cloc = 0) then is_environment.locate_pos := 1
  else is_environment.locate_pos := cloc;
  is_environment.keystroke := key;
  is_environment.insert_mode := ins;
  InputStr := s;

  _dbg_leave; //EXIT //InputStr
end;
