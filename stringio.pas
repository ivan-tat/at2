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
{$MODESWITCH CVAR}
{$L stringio.o}
interface

type
  tCHARSET = Set of Char;

var
  DEC_NUM_CHARSET: set of Char; cvar; external;
  HEX_NUM_CHARSET: set of Char; cvar; external;

{$IFNDEF ADT2PLAY}
function CStrLen(str: String): Byte; cdecl; external;
function CStr2Len(str: String): Byte; cdecl; external;
function C3StrLen(str: String): Byte; cdecl; external;
{$ENDIF} // NOT DEFINED(ADT2PLAY)

function byte2hex(value: Byte): String; cdecl; external;
function byte2dec(value: Byte): String; cdecl; external;
function Capitalize(str: String): String; cdecl; external;
function Upper(str: String): String; cdecl; external;
function Lower(str: String): String; cdecl; external;
function iCASE(str: String): String;
function RotStrL(str1,str2: String; shift: Byte): String;
function RotStrR(str1,str2: String; shift: Byte): String;
function ExpStrL(str: String; size: Byte; chr: Char): String; cdecl; external;
function ExpStrR(str: String; size: Byte; chr: Char): String; cdecl; external;
function ExpC2StrL(str: String; size: Byte; chr: Char): String;
function ExpC2StrR(str: String; size: Byte; chr: Char): String;
function ExpC3StrL(str: String; size: Byte; chr: Char): String;
function ExpC3StrR(str: String; size: Byte; chr: Char): String;
function CenterStr(str: String; size: Byte): String;
function DietStr(str: String; size: Byte): String; cdecl; external;
function CutStr(str: String): String; cdecl; external;
function CutStrL(str: String; margin: Byte): String;
function CutStrR(str: String; margin: Byte): String; cdecl; external;
function FlipStr(str: String): String;
function FilterStr(str: String; chr0,chr1: Char): String; cdecl; external;
function FilterStr1(str: String; chr0: Char): String;
function FilterStr2(str: String; var chr0: tCHARSET; chr1: Char): String; cdecl; external;
function Num2str(num: Longint; base: Byte): String; cdecl; external;
function Str2num(str: String; base: Byte): Longint; cdecl; external;
function Bpm2str(bpm: Real): String;

function SameName(mask,str: String): Boolean;
function PathOnly(path: String): String;
function NameOnly(path: String): String;
function BaseNameOnly(path: String): String;
function ExtOnly(path: String): String;

procedure StringIO_Init;

procedure init_StringIO; cdecl; external;

implementation

uses
  DOS,
  StrUtils,
  AdT2unit,
  AdT2sys;

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

function Bpm2str(bpm: Real): String;
begin
  If (bpm < 1000) then
    Bpm2str := Num2str(Trunc(bpm),10)+'.'+Num2str(Trunc((bpm-Trunc(bpm))*10),10)
  else
    Bpm2str := Num2str(Round(bpm),10);
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
  init_StringIO;
end;

begin
  init_StringIO;
end.
