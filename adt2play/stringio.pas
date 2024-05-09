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

function byte2hex(value: Byte): String; cdecl; external;
function byte2dec(value: Byte): String; cdecl; external;
function Capitalize(str: String): String; cdecl; external;
function Upper(str: String): String; cdecl; external;
function Lower(str: String): String; cdecl; external;
function ExpStrL(str: String; size: Byte; chr: Char): String; cdecl; external;
function ExpStrR(str: String; size: Byte; chr: Char): String; cdecl; external;
function DietStr(str: String; size: Byte): String; cdecl; external;
function CutStr(str: String): String; cdecl; external;
function FilterStr(str: String; chr0,chr1: Char): String; cdecl; external;
function Num2str(num: Longint; base: Byte): String; cdecl; external;
function Str2num(str: String; base: Byte): Longint; cdecl; external;
function PathOnly(path: String): String;
function NameOnly(path: String): String;
function BaseNameOnly(path: String): String;
function ExtOnly(path: String): String;

procedure init_StringIO; cdecl; external;

implementation

uses
  DOS;

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
  If (Pos('\',path) <> 0) then
    begin
      FSplit(path,dir,name,ext);
      NameOnly := name+ext;
    end
  else NameOnly := path;
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
  ExtOnly := Lower(ext);
end;

begin
  init_StringIO;
end.
