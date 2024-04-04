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
interface

const
  Black   = $00;  DGray    = $08;
  Blue    = $01;  LBlue    = $09;
  Green   = $02;  LGreen   = $0a;
  Cyan    = $03;  LCyan    = $0b;
  Red     = $04;  LRed     = $0c;
  Magenta = $05;  LMagenta = $0d;
  Brown   = $06;  Yellow   = $0e;
  LGray   = $07;  White    = $0f;
  Blink   = $80;

const
  MAX_SCREEN_MEM_SIZE = 180*60*2;
  SCREEN_MEM_SIZE: Longint = MAX_SCREEN_MEM_SIZE;

type
  tSCREEN_MEM = array[0..PRED(MAX_SCREEN_MEM_SIZE)] of Byte;
  tSCREEN_MEM_PTR = ^tSCREEN_MEM;

var
  text_screen_shadow: tSCREEN_MEM;

const
  screen_ptr: Pointer = Addr(text_screen_shadow);
  v_seg:  Word = $0b800;
  v_ofs:  Word = 0;
  v_mode: Byte = $03;
  MaxLn:  Byte = 80;
  MaxCol: Byte = 25;

var
  cursor_backup: Longint;
  DispPg: Byte;

type
  tFADE  = (first,fadeOut,fadeIn);
  tDELAY = (fast,delayed);

type
  tFADE_BUF = Record
                action: tFADE;
                pal0: array[0..255] of Record r,g,b: Byte end;
                pal1: array[0..255] of Record r,g,b: Byte end;
              end;
const
  fade_speed: Byte = 63;

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

procedure initialize;
begin
  asm
        mov     ah,0fh
        int     10h
        and     al,7fh
        mov     v_mode,al
        mov     DispPg,bh
  end;

  MaxCol := MEM[0:$44a];
  MaxLn := SUCC(MEM[0:$484]);
  FillWord(screen_ptr^,MAX_SCREEN_MEM_SIZE DIV 2,$0700);
end;

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

procedure GetVideoState(var data: tVIDEO_STATE);
begin
  asm
        mov     ah,0fh
        int     10h
        and     al,7fh
        mov     v_mode,al
        mov     DispPg,bh
  end;

  dosmemget(v_seg,v_ofs,data.screen,MAX_SCREEN_MEM_SIZE);
  data.cursor := GetCursor;
  data.font := MEMW[Seg0040:$85];
  data.v_seg := $0b800;
  data.v_ofs := MEM[Seg0040:$4e];
  data.DispPg := DispPg;
  data.v_mode := v_mode;
  data.MaxCol := MEM[Seg0040:$4a];
  data.MaxLn := SUCC(MEM[Seg0040:$84]);

  dosmemget(Seg0040,0,bios_data_backup,168);
  dos_mem_adr := global_dos_alloc(SizeOf(tVIDEO_STATE(data).data));
  dos_sel := WORD(dos_mem_adr);
  dos_seg := WORD(dos_mem_adr SHR 16);
  regs.cx := 7;
  regs.es := dos_seg;
  regs.ax := $1c01;
  RealIntr($10,regs);
  dosmemput(Seg0040,0,bios_data_backup,168);
  dosmemget(dos_seg,0,tVIDEO_STATE(data).data,SizeOf(tVIDEO_STATE(data).data));
  global_dos_free(dos_sel);
end;

procedure SetVideoState(var data: tVIDEO_STATE; restore_screen: Boolean);
begin
  v_seg  := data.v_seg;
  v_ofs  := data.v_ofs;
  v_mode := data.v_mode;
  DispPg := data.DispPg;

  asm
        movzx   ax,v_mode
        mov     bh,DispPg
        int     10h
  end;

  MaxCol := data.MaxCol;
  MaxLn := data.MaxLn;

  dosmemget(Seg0040,0,bios_data_backup,168);
  dos_mem_adr := global_dos_alloc(SizeOf(tVIDEO_STATE(data).data));
  dos_sel := WORD(dos_mem_adr);
  dos_seg := WORD(dos_mem_adr SHR 16);
  dosmemput(dos_seg,0,tVIDEO_STATE(data).data,SizeOf(tVIDEO_STATE(data).data));

  regs.cx := 7;
  regs.es := dos_seg;
  regs.ax := $1c02;
  RealIntr($10,regs);
  global_dos_free(dos_sel);
  dosmemput(Seg0040,0,bios_data_backup,168);

  MEM[Seg0040:$4e] := data.v_ofs;
  MEM[Seg0040:$4a] := MaxCol;
  MEM[Seg0040:$84] := PRED(MaxLn);

  Case data.font of
     8:  asm mov ax,1112h; xor bl,bl; int 10h end;
    14:  asm mov ax,1111h; xor bl,bl; int 10h end;
    else asm mov ax,1114h; xor bl,bl; int 10h end;
  end;

  SetCursor(data.cursor);
  If restore_screen then
    dosmemput(v_seg,v_ofs,data.screen,MAX_SCREEN_MEM_SIZE);
end;

begin
  initialize;
end.
