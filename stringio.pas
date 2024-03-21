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

unit StringIO;
{$S-,Q-,R-,V-,B-,X+}
{$PACKRECORDS 1}
interface

type
  tCHARSET = Set of Char;

const
  DEC_NUM_CHARSET = ['0'..'9'];
  HEX_NUM_CHARSET = ['0'..'9','a'..'f','A'..'F'];

function byte2hex(value: Byte): String;
function byte2dec(value: Byte): String;
function Capitalize(str: String): String;
function Upper(str: String): String;
function Lower(str: String): String;
function iCASE(str: String): String;
function RotStrL(str1,str2: String; shift: Byte): String;
function RotStrR(str1,str2: String; shift: Byte): String;
function ExpStrL(str: String; size: Byte; chr: Char): String;
function ExpStrR(str: String; size: Byte; chr: Char): String;
function ExpC2StrL(str: String; size: Byte; chr: Char): String;
function ExpC2StrR(str: String; size: Byte; chr: Char): String;
function ExpC3StrL(str: String; size: Byte; chr: Char): String;
function ExpC3StrR(str: String; size: Byte; chr: Char): String;
function CenterStr(str: String; size: Byte): String;
function DietStr(str: String; size: Byte): String;
function CutStr(str: String): String;
function CutStrL(str: String; margin: Byte): String;
function CutStrR(str: String; margin: Byte): String;
function FlipStr(str: String): String;
function FilterStr(str: String; chr0,chr1: Char): String;
function FilterStr1(str: String; chr0: Char): String;
function FilterStr2(str: String; chr0: tCHARSET; chr1: Char): String;
function Num2str(num: Longint; base: Byte): String;
function Str2num(str: String; base: Byte): Longint;
function Bpm2str(bpm: Real): String;

type
  tINPUT_STR_SETTING = Record
                         insert_mode,
                         replace_enabled,
                         append_enabled:  Boolean;
                         char_filter,
                         character_set,
                         valid_chars,
                         word_characters: tCHARSET;
                         terminate_keys:  array[1..50] of Word;
                       end;
type
  tINPUT_STR_ENVIRONMENT = Record
                             keystroke: Word;
                             locate_pos: Byte;
                             insert_mode: Boolean;
                             min_num: Dword;
                             max_num: Dword;
                             cur_str: String;
                             ext_proc: procedure;
                           end;
const
  is_setting: tINPUT_STR_SETTING =
    (insert_mode:     TRUE;
     replace_enabled: TRUE;
     append_enabled:  TRUE;
     char_filter:     [#32..#255];
     character_set:   [#32..#255];
     valid_chars:     [#32..#255];
     word_characters: ['A'..'Z','a'..'z','0'..'9','_'];
     terminate_keys:  ($011b,$1c0d,$0000,$0000,$0000,
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
  is_environment: tINPUT_STR_ENVIRONMENT;

function InputStr(s: String; x,y,ln,ln1: Byte; atr1,atr2: Byte): String;
function SameName(mask,str: String): Boolean;
function PathOnly(path: String): String;
function NameOnly(path: String): String;
function BaseNameOnly(path: String): String;
function ExtOnly(path: String): String;

procedure StringIO_Init;

implementation

uses
  DOS,
  StrUtils,
  AdT2unit,AdT2sys,AdT2keyb,
  TxtScrIO;

function byte2hex(value: Byte): String;

const
  data: array[0..15] of char = '0123456789ABCDEF';

begin
{$IFNDEF CPU64}
  asm
        mov     edi,@RESULT
        lea     ebx,[data]
        mov     al,2
        stosb
        mov     al,value
        xor     ah,ah
        mov     cl,16
        div     cl
        xlat
        stosb
        mov     al,ah
        xlat
        stosb
  end;
{$ELSE}
  byte2hex := data[value AND $0f0 SHR 4]+
              data[value AND $0f];
{$ENDIF}
end;

function byte2dec(value: Byte): String;

const
  data: array[0..9] of char = '0123456789';

begin
{$IFNDEF CPU64}
  asm
        mov     edi,@RESULT
        lea     ebx,[data]
        mov     al,value
        xor     ah,ah
        mov     cl,100
        div     cl
        mov     ch,ah
        xchg    ah,al
        or      ah,ah
        jz      @@1
        mov     al,3
        stosb
        xchg    ah,al
        xlat
        stosb
        mov     al,ch
        jmp     @@2
@@1:    mov     al,2
        stosb
        mov     al,value
@@2:    xor     ah,ah
        mov     cl,10
        div     cl
        xlat
        stosb
        mov     al,ah
        xlat
        stosb
  end;
{$ELSE}
  If (value < 100) then
    byte2dec := data[value DIV 10]+
                data[value MOD 10]
  else
    byte2dec := data[value DIV 100]+
                data[value MOD 100 DIV 10]+
                data[value MOD 100 MOD 10];
{$ENDIF}
end;

function Capitalize(str: String): String;
begin
  asm
        lea     esi,[str]
        mov     edi,@RESULT
        mov     al,[esi]
        inc     esi
        mov     [edi],al
        inc     edi
        xor     ecx,ecx
        mov     cl,al
        jecxz   @@4
        mov     al,[esi]
        inc     esi
        cmp     al,'a'
        jb      @@0
        cmp     al,'z'
        ja      @@0
        sub     al,20h
@@0:    mov     [edi],al
        inc     edi
@@1:    mov     ah,al
        mov     al,[esi]
        inc     esi
        cmp     ah,' '
        jnz     @@2
        cmp     al,'a'
        jb      @@2
        cmp     al,'z'
        ja      @@2
        sub     al,20h
        jmp     @@3
@@2:    cmp     al,'A'
        jb      @@3
        cmp     al,'Z'
        ja      @@3
        add     al,20h
@@3:    mov     [edi],al
        inc     edi
        loop    @@1
@@4:
  end;
end;

function Upper(str: String): String;
begin
{$IFNDEF CPU64}
  asm
        lea     esi,[str]
        mov     edi,@RESULT
        mov     al,[esi]
        inc     esi
        mov     [edi],al
        inc     edi
        xor     ecx,ecx
        mov     cl,al
        jecxz   @@3
@@1:    mov     al,[esi]
        inc     esi
        cmp     al,'a'
        jb      @@2
        cmp     al,'z'
        ja      @@2
        sub     al,20h
@@2:    mov     [edi],al
        inc     edi
        loop    @@1
@@3:
  end;
{$ELSE}
  Upper := UpCase(str);
{$ENDIF}
end;

function Lower(str: String): String;
begin
{$IFNDEF CPU64}
  asm
        lea     esi,[str]
        mov     edi,@RESULT
        mov     al,[esi]
        inc     esi
        mov     [edi],al
        inc     edi
        xor     ecx,ecx
        mov     cl,al
        jecxz   @@3
@@1:    mov     al,[esi]
        inc     esi
        cmp     al,'A'
        jb      @@2
        cmp     al,'Z'
        ja      @@2
        add     al,20h
@@2:    mov     [edi],al
        inc     edi
        loop    @@1
@@3:
  end;
{$ELSE}
  Lower := LowerCase(str);
{$ENDIF}
end;

{$IFNDEF CPU64}
function iCase(str: String): String;
begin
  asm
        lea     esi,[str]
        mov     edi,@RESULT
        mov     al,[esi]
        inc     esi
        mov     [edi],al
        inc     edi
        xor     ecx,ecx
        mov     cl,al
        jecxz   @@5
        push    edi
        push    ecx
@@1:    mov     al,[esi]
        inc     esi
        cmp     al,'a'
        jb      @@2
        cmp     al,'z'
        ja      @@2
        sub     al,20h
@@2:    mov     [edi],al
        inc     edi
        loop    @@1
        pop     ecx
        pop     edi
@@3:    mov     al,[edi]
        cmp     al,'i'-20h
        jnz     @@4
        add     al,20h
@@4:    mov     [edi],al
        inc     edi
        loop    @@3
@@5:
  end;
end;
{$ELSE}
function iCase(str: String): String;
begin
  iCase := ReplaceStr(Upper(str),'I','i');
end;
{$ENDIF}

function RotStrL(str1,str2: String; shift: Byte): String;
begin
  RotStrL := Copy(str1,shift+1,Length(str1)-shift)+
             Copy(str2,1,shift);
end;

function RotStrR(str1,str2: String; shift: Byte): String;
begin
  RotStrR := Copy(str2,Length(str2)-shift+1,shift)+
             Copy(str1,1,Length(str1)-shift);
end;

function ExpStrL(str: String; size: Byte; chr: Char): String;
begin
{$IFNDEF CPU64}
  asm
        lea     esi,[str]
        mov     edi,@RESULT
        cld
        xor     ecx,ecx
        lodsb
        cmp     al,size
        jge     @@1
        mov     ah,al
        mov     al,size
        stosb
        mov     al,ah
        mov     cl,size
        sub     cl,al
        mov     al,chr
        rep     stosb
        mov     cl,ah
        rep     movsb
        jmp     @@2
@@1:    stosb
        mov     cl,al
        rep     movsb
@@2:
  end;
{$ELSE}
  While (Length(str) < size) do
    str := chr+str;
  ExpStrL := str;
{$ENDIF}
end;

function ExpStrR(str: String; size: Byte; chr: Char): String;
begin
{$IFNDEF CPU64}
  asm
        lea     esi,[str]
        mov     edi,@RESULT
        cld
        xor     ecx,ecx
        lodsb
        cmp     al,size
        jge     @@1
        mov     ah,al
        mov     al,size
        stosb
        mov     cl,ah
        rep     movsb
        mov     al,ah
        mov     cl,size
        sub     cl,al
        mov     al,chr
        rep     stosb
        jmp     @@2
@@1:    stosb
        mov     cl,al
        rep     movsb
@@2:
  end;
{$ELSE}
  While (Length(str) < size) do
    str := str+chr;
  ExpStrR := str;
{$ENDIF}
end;

function ExpC2StrL(str: String; size: Byte; chr: Char): String;
begin
  While (CStr2Len(str) < size) do
    str := chr+str;
  ExpC2StrL := str;
end;

function ExpC2StrR(str: String; size: Byte; chr: Char): String;
begin
  While (CStr2Len(str) < size) do
    str := str+chr;
  ExpC2StrR := str;
end;

function ExpC3StrL(str: String; size: Byte; chr: Char): String;
begin
  While (C3StrLen(str) < size) do
    str := chr+str;
  ExpC3StrL := str;
end;

function ExpC3StrR(str: String; size: Byte; chr: Char): String;
begin
  While (C3StrLen(str) < size) do
    str := str+chr;
  ExpC3StrR := str;
end;

function CenterStr(str: String; size: Byte): String;

var
  flag: Boolean;

begin
  flag := FALSE;
  While (Length(str) < size) do
    begin
      If flag then
        str := ' ' + str
      else str := str + ' ';
      flag := NOT flag;
    end;
  CenterStr := str;
end;

function DietStr(str: String; size: Byte): String;
begin
  If (Length(str) <= size) then
    begin
      DietStr := str;
      EXIT;
    end;

  Repeat
    Delete(str,size DIV 2,1)
  until (Length(str)+3 = size);

  Insert('...',str,size DIV 2);
  DietStr := str
end;

function CutStr(str: String): String;
begin
  While (BYTE(str[0]) <> 0) and (str[1] = ' ') do
    Delete(str,1,1);
  While (BYTE(str[0]) <> 0) and (str[BYTE(str[0])] = ' ') do
    Delete(str,BYTE(str[0]),1);
  CutStr := str;
end;

function CutStrL(str: String; margin: Byte): String;

var
  idx: Byte;

begin
  If (margin = 0) then margin := Length(str)
  else If (margin > Length(str)) then
         margin := Length(str);
  idx := 0;
  While (idx+1 <= margin) and (str[idx+1] = ' ') do
    Inc(idx);
  If (idx <> 0) then Delete(str,1,idx);
  CutStrL := str;
end;

function CutStrR(str: String; margin: Byte): String;

var
  idx: Byte;

begin
  If (margin > Length(str)) then
    margin := Length(str);
  idx := 0;
  While (str[BYTE(str[0])-idx] = ' ') and
        (BYTE(str[0])-idx >= margin) do
    Inc(idx);
  Dec(BYTE(str[0]),idx);
  CutStrR := str;
end;

{$IFNDEF CPU64}
function FlipStr(str: String): String;
begin
   asm
        lea     esi,[str]
        mov     edi,@RESULT
        mov     al,[esi]
        inc     esi
        mov     [edi],al
        inc     edi
        dec     edi
        xor     ecx,ecx
        mov     cl,al
        jecxz   @@2
        add     edi,ecx
@@1:    mov     al,[esi]
        inc     esi
        mov     [edi],al
        dec     edi
        loop    @@1
@@2:
  end;
end;
{$ELSE}
function FlipStr(str: String): String;

var
  idx: Byte;
  result: String;

begin
  result := '';
  For idx := 1 to Length(str) do
    result := str[idx]+result;
  FlipStr := result;
end;
{$ENDIF}

{$IFNDEF CPU64}
function FilterStr(str: String; chr0,chr1: Char): String;
begin
  asm
        lea     esi,[str]
        mov     edi,@RESULT
        mov     al,[esi]
        inc     esi
        mov     [edi],al
        inc     edi
        xor     ecx,ecx
        mov     cl,al
        jecxz   @@3
@@1:    mov     al,[esi]
        inc     esi
        cmp     al,chr0
        jnz     @@2
        mov     al,chr1
@@2:    mov     [edi],al
        inc     edi
        loop    @@1
@@3:
  end;
end;
{$ELSE}
function FilterStr(str: String; chr0,chr1: Char): String;

var
  idx: Byte;

begin
  For idx := 1 to Length(str) do
    If (str[idx] = chr0) then
      str[idx] := chr1;
  FilterStr := str;
end;
{$ENDIF}

{$IFNDEF CPU64}
function FilterStr1(str: String; chr0: Char): String;
begin
  asm
        lea     esi,[str]
        mov     edi,@RESULT
        mov     al,[esi]
        inc     esi
        inc     edi
        xor     ecx,ecx
        mov     cl,al
        mov     ebx,ecx
        jecxz   @@4
@@1:    mov     al,[esi]
        inc     esi
        cmp     al,chr0
        jnz     @@2
        dec     ebx
        jmp     @@3
@@2:    mov     [edi],al
        inc     edi
@@3:    loop    @@1
@@4:    mov     eax,ebx
        mov     edi,@RESULT
        mov     [edi],al
  end;
end;
{$ELSE}
function FilterStr1(str: String; chr0: Char): String;

var
  idx: Byte;
  result: String;

begin
  result := '';
  For idx := 1 to Length(str) do
    If (str[idx] <> chr0) then
      result := result+str[idx];
  FilterStr1 := result;
end;
{$ENDIF}

const
  _treat_char: array[$80..$a5] of Char =
    'CueaaaaceeeiiiAAE_AooouuyOU_____aiounN';

function FilterStr2(str: String; chr0: tCHARSET; chr1: Char): String;

var
  temp: Byte;

begin
  For temp := 1 to Length(str) do
    If NOT (str[temp] in chr0) then
      If (str[temp] >= #128) and (str[temp] <= #165) then
        str[temp] := _treat_char[BYTE(str[temp])]
      else If (str[temp] = #0) then str[temp] := ' '
           else str[temp] := chr1;
  FilterStr2 := str;
end;

{$IFNDEF CPU64}
function Num2str(num: Longint; base: Byte): String;

const
  hexa: array[0..PRED(16)+32] of Char = '0123456789ABCDEF'+
                                        #0#0#0#0#0#0#0#0#0#0#0#0#0#0#0#0;
begin
  asm
        xor     eax,eax
        xor     edx,edx
        xor     edi,edi
        xor     esi,esi
        mov     eax,num
        xor     ebx,ebx
        mov     bl,base
        cmp     bl,2
        jb      @@3
        cmp     bl,16
        ja      @@3
        mov     edi,32
@@1:    dec     edi
        xor     edx,edx
        div     ebx
        mov     esi,edx
        mov     dl,byte ptr [hexa+esi]
        mov     byte ptr [hexa+edi+16],dl
        and     eax,eax
        jnz     @@1
        mov     esi,edi
        mov     ecx,32
        sub     ecx,edi
        mov     edi,@RESULT
        mov     al,cl
        stosb
@@2:    mov     al,byte ptr [hexa+esi+16]
        stosb
        inc     esi
        loop    @@2
        jmp     @@4
@@3:    mov     edi,@RESULT
        xor     al,al
        stosb
@@4:
  end;
end;
{$ELSE}
function Num2str(num: Longint; base: Byte): String;

const
  hexa: array[0..PRED(16)] of Char = '0123456789ABCDEF';

var
  result: String;

begin
  result := '';
  If (base >= 2) and (base <= 16) then
    While (num > 0) do
      begin
        result := hexa[num MOD base]+result;
        num := num DIV base;
      end;
  If (result = '') then Num2str := '0'
  else Num2str := result;
end;
{$ENDIF}

const
  digits: array[0..15] of Char = '0123456789ABCDEF';

function Digit2index(digit: Char): Byte;

var
  index: Byte;

begin
  digit := UpCase(digit);
  index := 15;
  While (index > 0) and (digit <> digits[index]) do Dec(index);
  Digit2index := Index;
end;

function position_value(position,base: Byte): Longint;

var
  value: Longint;
  index: Byte;

begin
  value := 1;
  For index := 2 to position do value := value*base;
  position_value := value;
end;

function Str2num(str: String; base: Byte): Longint;

var
  value: Longint;
  index: Byte;

begin
  value := 0;
  If (base in [2,10,16]) then
    For index := 1 to Length(str) do
      Inc(value,Digit2index(str[index])*
                position_value(Length(str)-index+1,base));
  Str2num := value;
end;

function Bpm2str(bpm: Real): String;
begin
  If (bpm < 1000) then
    Bpm2str := Num2str(Trunc(bpm),10)+'.'+Num2str(Trunc((bpm-Trunc(bpm))*10),10)
  else
    Bpm2str := Num2str(Round(bpm),10);
end;

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
{$IFDEF GO32V2}
  _last_debug_str_ := _debug_str_;
  _debug_str_ := 'STRINGIO.PAS:InputStr';
{$ENDIF}
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
    If keypressed then key := getkey else GOTO _end;
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
end;

function SameName(mask,str: String): Boolean;
begin
  If (Length(mask) > Length(str)) then
    While (mask[Length(mask)] in ['*','?']) and (Length(mask) > Length(str)) do
      Dec(mask[0]);
  SameName := IsWild(str,mask,FALSE);
end;

var
  dir:  DirStr;
  name: NameStr;
  ext:  ExtStr;

function PathOnly(path: String): String;
begin
  FSplit(path,dir,name,ext);
  PathOnly := dir;
end;

function NameOnly(path: String): String;
begin
  FSplit(path,dir,name,ext);
  NameOnly := name+ext;
end;

function BaseNameOnly(path: String): String;
begin
  FSplit(path,dir,name,ext);
  BaseNameOnly := name;
end;

function ExtOnly(path: String): String;
begin
  FSplit(path,dir,name,ext);
  Delete(ext,1,1);
  ExtOnly := Lower_filename(ext);
end;

procedure StringIO_Init;
begin
  is_environment.locate_pos := 1;
  is_setting.char_filter := _valid_characters;
  is_setting.valid_chars := _valid_characters;
  is_environment.min_num := 0;
  is_environment.max_num := SizeOf(DWORD);
  is_environment.cur_str := '';
  is_environment.ext_proc := NIL;
end;

end.
