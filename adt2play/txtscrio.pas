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
{$MODESWITCH CVAR}
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
  v_seg:  Word; cvar; external;
  v_ofs:  Word; cvar; external;
  v_mode: Byte; cvar; external;
  MaxCol: Byte; cvar; external;
  MaxLn:  Byte; cvar; external;

  cursor_backup: Longint; cvar; external;
  DispPg: Byte; cvar; external;

{$I txtscrio/go32/pas/fade.pas}

type
  tVIDEO_STATE = Record
                   cursor: Longint;
                   font: Byte;
                   MaxLn,MaxCol,v_mode,DispPg: Byte;
                   v_seg,v_ofs: Word;
                   screen: tSCREEN_MEM;
                   data: array[0..PRED(4096)] of Byte;
                 end;

procedure ShowC3Str(dest: tSCREEN_MEM_PTR; x,y: Byte; str: String; atr1,atr2,atr3: Byte);
function  iVGA:  Boolean;
function  WhereX: Byte;
function  WhereY: Byte;
procedure GotoXY(x,y: Byte);
function  GetCursor: Longint;
procedure SetCursor(cursor: Longint);
procedure ThinCursor;
procedure WideCursor;
procedure HideCursor;
function  GetCursorShape: Word;
procedure SetCursorShape(shape: Word);
procedure GetRGBitem(color: Byte; var red,green,blue: Byte);
procedure SetRGBitem(color: Byte; red,green,blue: Byte);
procedure GetPalette(var pal; first,last: Word);
procedure SetPalette(var pal; first,last: Word);
procedure WaitRetrace;
procedure VgaFade(var data: tFADE_BUF; fade: tFADE; delay: tDELAY);
procedure GetVideoState(var data: tVIDEO_STATE);
procedure SetVideoState(var data: tVIDEO_STATE; restore_screen: Boolean);

implementation
uses
  GO32;

var
  absolute_pos: Word;

{$I txtscrio/i386/DupChar.pas}
{$I txtscrio/i386/ShowC3Str.pas}
{$I txtscrio/i386/ScreenMemCopy.pas}

{$I txtscrio/pas/WhereX.pas}
{$I txtscrio/pas/WhereY.pas}
{$I txtscrio/pas/GotoXY.pas}
{$I txtscrio/pas/GetCursor.pas}
{$I txtscrio/pas/SetCursor.pas}
{$I txtscrio/pas/ThinCursor.pas}
{$I txtscrio/pas/WideCursor.pas}
{$I txtscrio/pas/HideCursor.pas}
{$I txtscrio/pas/GetCursorShape.pas}
{$I txtscrio/pas/SetCursorShape.pas}
{$I txtscrio/go32/pas/initialize.pas}
{$I txtscrio/go32/pas/iVGA.pas}
{$I txtscrio/go32/pas/GetRGBitem.pas}
{$I txtscrio/go32/pas/SetRGBitem.pas}
{$I txtscrio/go32/pas/GetPalette.pas}
{$I txtscrio/go32/pas/SetPalette.pas}
{$I txtscrio/go32/pas/WaitRetrace.pas}
{$I txtscrio/go32/pas/VgaFade.pas}

var
  regs: tRealRegs;
  dos_sel,dos_seg: Word;
  dos_mem_adr: Dword;
  bios_data_backup: array[0..167] of Byte;

{$I txtscrio/go32/pas/GetVideoState.pas}
{$I txtscrio/go32/pas/SetVideoState.pas}

begin
  initialize;
end.
