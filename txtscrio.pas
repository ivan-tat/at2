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

var
  SCREEN_MEM_SIZE: Longint; cvar; external;
{$IFNDEF ADT2PLAY}
  SCREEN_RES_X: Word; cvar; external;
  SCREEN_RES_Y: Word; cvar; external;
  MAX_COLUMNS: Byte; cvar; external;
  MAX_ROWS: Byte; cvar; external;
  MAX_TRACKS: Byte; cvar; external;
  MAX_ORDER_COLS: Byte; cvar; external;
  MAX_PATTERN_ROWS: Byte; cvar; external;
  INSCTRL_xshift: Byte; cvar; external;
  INSCTRL_yshift: Shortint; cvar; external;
  INSEDIT_yshift: Byte; cvar; external;
  PATTORD_xshift: Byte; cvar; external;
  GOTOXY_xshift: Byte; cvar; external;
{$ENDIF} // NOT DEFINED(ADT2PLAY)

type
  tSCREEN_MEM = array[0..PRED(MAX_SCREEN_MEM_SIZE)] of Byte;
  tSCREEN_MEM_PTR = ^tSCREEN_MEM;

var
  text_screen_shadow:   tSCREEN_MEM; cvar; external;
{$IFNDEF ADT2PLAY}
  temp_screen:          tSCREEN_MEM; cvar; external;
  temp_screen2:         tSCREEN_MEM; cvar; external;
  screen_backup:        tSCREEN_MEM; cvar; external;
  scr_backup:           tSCREEN_MEM; cvar; external;
  scr_backup2:          tSCREEN_MEM; cvar; external;
  screen_mirror:        tSCREEN_MEM; cvar; external;
  screen_emulator:      tSCREEN_MEM; cvar; external;
  centered_frame_vdest: tSCREEN_MEM_PTR; cvar; external;
{$ENDIF} // NOT DEFINED(ADT2PLAY)

  screen_ptr:          Pointer; cvar; external;
{$IFNDEF ADT2PLAY}
  ptr_temp_screen:     Pointer; cvar; external;
  ptr_temp_screen2:    Pointer; cvar; external;
  ptr_screen_backup:   Pointer; cvar; external;
  ptr_scr_backup:      Pointer; cvar; external;
  ptr_scr_backup2:     Pointer; cvar; external;
  ptr_screen_mirror:   Pointer; cvar; external;
  ptr_screen_emulator: Pointer; cvar; external;

  move_to_screen_data: Pointer; cvar; external;
  move_to_screen_area: array[1..4] of Byte; cvar; external;
  move_to_screen_routine: procedure; cdecl; cvar; external;

  program_screen_mode: Byte; cvar; external;
{$ENDIF} // NOT DEFINED(ADT2PLAY)

  MaxCol: Byte; cvar; external;
  MaxLn:  Byte; cvar; external;
{$IFNDEF ADT2PLAY}
  hard_maxcol: Byte; cvar; external;
  hard_maxln:  Byte; cvar; external;
  work_MaxCol: Byte; cvar; external;
  work_MaxLn:  Byte; cvar; external;
  scr_font_width: Byte; cvar; external;
  scr_font_height: Byte; cvar; external;

  area_x1: Byte; cvar; external;
  area_y1: Byte; cvar; external;
  area_x2: Byte; cvar; external;
  area_y2: Byte; cvar; external;
  scroll_pos0: Byte; cvar; external;
  scroll_pos1: Byte; cvar; external;
  scroll_pos2: Byte; cvar; external;
  scroll_pos3: Byte; cvar; external;
  scroll_pos4: Byte; cvar; external;
{$ENDIF} // NOT DEFINED(ADT2PLAY)

  cursor_backup: Longint; cvar; external;

{$IFNDEF ADT2PLAY}
procedure ShowStr(dest: tSCREEN_MEM_PTR; x,y: Byte; str: String; attr: Byte); cdecl; external;
procedure ShowVStr(dest: tSCREEN_MEM_PTR; x,y: Byte; str: String; attr: Byte); cdecl; external;
procedure ShowCStr(dest: tSCREEN_MEM_PTR; x,y: Byte; str: String; atr1,atr2: Byte); cdecl; external;
procedure ShowCStr2(dest: tSCREEN_MEM_PTR; x,y: Byte; str: String; atr1,atr2: Byte); cdecl; external;
procedure ShowVCStr(dest: tSCREEN_MEM_PTR; x,y: Byte; str: String; atr1,atr2: Byte); cdecl; external;
procedure ShowVCStr2(dest: tSCREEN_MEM_PTR; x,y: Byte; str: String; atr1,atr2: Byte); cdecl; external;
{$ENDIF} // NOT DEFINED(ADT2PLAY)
procedure ShowC3Str(dest: tSCREEN_MEM_PTR; x,y: Byte; str: String; atr1,atr2,atr3: Byte); cdecl; external;
{$IFNDEF ADT2PLAY}
procedure ShowVC3Str(dest: tSCREEN_MEM_PTR; x,y: Byte; str: String; atr1,atr2,atr3: Byte); cdecl; external;
procedure ShowC4Str(dest: tSCREEN_MEM_PTR; x,y: Byte; str: String; atr1,atr2,atr3,atr4: Byte); cdecl; external;
procedure show_str(xpos,ypos: Byte; str: String; color: Byte); cdecl; external;
procedure show_cstr(xpos,ypos: Byte; str: String; attr1,attr2: Byte); cdecl; external;
procedure show_cstr_alt(xpos,ypos: Byte; str: String; attr1,attr2: Byte); cdecl; external;
procedure show_vstr(xpos,ypos: Byte; str: String; color: Byte); cdecl; external;
procedure show_vcstr(xpos,ypos: Byte; str: String; attr1,attr2: Byte); cdecl; external;

function  CStrLen(str: String): Byte; cdecl; external;
function  CStr2Len(str: String): Byte; cdecl; external;
function  C3StrLen(str: String): Byte; cdecl; external;

procedure ScreenMemCopy(source,dest: tSCREEN_MEM_PTR); cdecl; external;
procedure move2screen; cdecl; external;
procedure move2screen_alt; cdecl; external;
procedure TxtScrIO_Init; cdecl; external;
function  is_default_screen_mode: Boolean; cdecl; external;
{$IFDEF GO32V2}
function  is_VESA_emulated_mode: Boolean; cdecl; external;
function  get_VESA_emulated_mode_idx: Byte; cdecl; external;
{$ENDIF}
function  is_scrollable_screen_mode: Boolean; cdecl; external;

type
  tFRAME_SETTING = Record
                     shadow_enabled,
                     wide_range_type,
                     zooming_enabled,
                     update_area: Boolean;
                   end;
var
  fr_setting: tFRAME_SETTING; cvar; external;

procedure Frame(dest: tSCREEN_MEM_PTR; x1,y1,x2,y2,atr1: Byte;
                title: String; atr2: Byte; border: String); cdecl; external;
{$ENDIF} // NOT DEFINED(ADT2PLAY)

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

{$IFNDEF ADT2PLAY}
type
  tCUSTOM_VIDEO_MODE = 0..52;
{$ENDIF} // NOT DEFINED(ADT2PLAY)

function  iVGA: Boolean; cdecl; external;
procedure initialize; cdecl; external;
{$IFNDEF ADT2PLAY}
procedure ResetMode; cdecl; external;
procedure SetCustomVideoMode(vmode: tCUSTOM_VIDEO_MODE); cdecl; external;
{$ENDIF} // NOT DEFINED(ADT2PLAY)
procedure GetRGBitem(color: Byte; var red,green,blue: Byte); cdecl; external;
procedure SetRGBitem(color: Byte; red,green,blue: Byte); cdecl; external;
procedure GetPalette(var pal; first,last: Word); cdecl; external;
procedure SetPalette(var pal; first,last: Word); cdecl; external;
procedure WaitRetrace; cdecl; external;

type
  tFADE  = (first,fadeOut,fadeIn);
  tDELAY = (fast,delayed);

type
  tFADE_BUF = Record
                action: tFADE;
                pal0: array[0..255] of Record r,g,b: Byte end;
                pal1: array[0..255] of Record r,g,b: Byte end;
              end;

var
  fade_speed: Byte; cvar; external;

procedure VgaFade(var data: tFADE_BUF; fade: tFADE; delay: tDELAY); cdecl; external;
{$IFNDEF ADT2PLAY}
procedure RefreshEnable; cdecl; external;
procedure RefreshDisable; cdecl; external;
procedure Split2Static; cdecl; external;
procedure SplitScr(line: Word); cdecl; external;
procedure SetSize(columns,lines: Word); cdecl; external;
procedure SetTextDisp(x,y: Word); cdecl; external;
procedure set_vga_txtmode_80x25; cdecl; external;
procedure set_svga_txtmode_100x38; cdecl; external;
procedure set_svga_txtmode_128x48; cdecl; external;
procedure set_custom_svga_txtmode; cdecl; external;

type
  VGA_REGISTER = Record
                   port: Word;
                   idx: Byte;
                   val: Byte;
                 end;
type
  VGA_REG_DATA = array[1..29] of VGA_REGISTER;

var
  svga_txtmode_cols: Byte; cvar; external;
  svga_txtmode_rows: Byte; cvar; external;
  svga_txtmode_regs: VGA_REG_DATA; cvar; external;

{$ELSE} // DEFINED(ADT2PLAY)

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

{$ENDIF} // DEFINED(ADT2PLAY)

{$ENDIF}

implementation

uses
{$IFDEF GO32V2}
{$IFNDEF ADT2PLAY}
  CRT,
{$ENDIF} // NOT DEFINED(ADT2PLAY)
  GO32,
  VGA,
{$ENDIF}
  pascal,
{$IFNDEF ADT2PLAY}
  AdT2unit,
  AdT2sys,
  AdT2ext2,
  DialogIO;
{$ELSE} // DEFINED(ADT2PLAY)
  common;
{$ENDIF} // DEFINED(ADT2PLAY)

{$IFDEF GO32V2}
begin
  initialize;
{$ENDIF}
end.
