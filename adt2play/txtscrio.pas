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

unit TxtScrIO;
{$S-,Q-,R-,V-,B-,X+}
{$PACKRECORDS 1}
{$L txtscrio.o}
interface

{$I txtscrio/pas/colors.pas}

const
  MAX_SCREEN_MEM_SIZE = 180*60*2;
var
  SCREEN_MEM_SIZE: Longint; cvar; external;

type
  tSCREEN_MEM = array[0..PRED(MAX_SCREEN_MEM_SIZE)] of Byte;
  tSCREEN_MEM_PTR = ^tSCREEN_MEM;

var
  text_screen_shadow: tSCREEN_MEM; cvar; external;

  screen_ptr: Pointer; cvar; external;

  MaxCol: Byte; cvar; external;
  MaxLn:  Byte; cvar; external;

  cursor_backup: Longint; cvar; external;

procedure ShowC3Str(dest: tSCREEN_MEM_PTR; x,y: Byte; str: String; atr1,atr2,atr3: Byte); cdecl; external;

function  WhereX: Byte; cdecl; external;
function  WhereY: Byte; cdecl; external;
procedure GotoXY(x,y: Byte); cdecl; external;
function  GetCursor: Longint; cdecl; external;
procedure SetCursor(cursor: Longint); cdecl; external;
procedure ThinCursor; cdecl; external;
procedure WideCursor; cdecl; external;
procedure HideCursor; cdecl; external;
function  GetCursorShape: Word; cdecl; external;
procedure SetCursorShape(shape: Word); cdecl; external;

{$IFDEF GO32V2}

var
  v_seg:  Word; cvar; external;
  v_ofs:  Word; cvar; external;
  v_mode: Byte; cvar; external;

  DispPg: Byte; cvar; external;

function  iVGA: Boolean; cdecl; external;
procedure initialize; cdecl; external;
procedure GetRGBitem(color: Byte; var red,green,blue: Byte); cdecl; external;
procedure SetRGBitem(color: Byte; red,green,blue: Byte); cdecl; external;
procedure GetPalette(var pal; first,last: Word); cdecl; external;
procedure SetPalette(var pal; first,last: Word); cdecl; external;
procedure WaitRetrace; cdecl; external;

{$I txtscrio/go32/pas/fade.pas}

procedure VgaFade(var data: tFADE_BUF; fade: tFADE; delay: tDELAY); cdecl; external;

type
  tVIDEO_STATE = Record
                   cursor: Longint;
                   font: Byte;
                   MaxLn,MaxCol,v_mode,DispPg: Byte;
                   v_seg,v_ofs: Word;
                   screen: tSCREEN_MEM;
                   data: array[0..PRED(4096)] of Byte;
                 end;

procedure GetVideoState(var data: tVIDEO_STATE); cdecl; external;
procedure SetVideoState(var data: tVIDEO_STATE; restore_screen: Boolean); cdecl; external;

{$ENDIF}

implementation

uses
{$IFDEF GO32V2}
  GO32,
{$ENDIF}
  pascal,
  common;

begin
  initialize;
end.
