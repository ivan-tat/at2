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

unit AdT2keyb;
{$S-,Q-,R-,V-,B-,X+}
{$PACKRECORDS 1}
{$L adt2keyb.o}
interface

const
    SC_ESCAPE       = $01;
    SC_1            = $02;
    SC_2            = $03;
    SC_3            = $04;
    SC_4            = $05;
    SC_5            = $06;
    SC_6            = $07;
    SC_7            = $08;
    SC_8            = $09;
    SC_9            = $0A;
    SC_0            = $0B;
    SC_MINUS        = $0C;
    SC_EQUALS       = $0D;
    SC_BACKSPACE    = $0E;
    SC_TAB          = $0F;
    SC_Q            = $10;
    SC_W            = $11;
    SC_E            = $12;
    SC_R            = $13;
    SC_T            = $14;
    SC_Y            = $15;
    SC_U            = $16;
    SC_I            = $17;
    SC_O            = $18;
    SC_P            = $19;
    SC_LEFTBRACKET  = $1A;
    SC_RIGHTBRACKET = $1B;
    SC_ENTER        = $1C;
{$IFDEF GO32V2}
    SC_CTRL         = $1D;
{$ENDIF} // DEFINED(GO32V2)
    SC_A            = $1E;
    SC_S            = $1F;
    SC_D            = $20;
    SC_F            = $21;
    SC_G            = $22;
    SC_H            = $23;
    SC_J            = $24;
    SC_K            = $25;
    SC_L            = $26;
    SC_SEMICOLON    = $27;
    SC_QUOTE        = $28;
    SC_TILDE        = $29;
    SC_LSHIFT       = $2A;
    SC_BACKSLASH    = $2B;
    SC_Z            = $2C;
    SC_X            = $2D;
    SC_C            = $2E;
    SC_V            = $2F;
    SC_B            = $30;
    SC_N            = $31;
    SC_M            = $32;
    SC_COMMA        = $33;
    SC_PERIOD       = $34;
    SC_SLASH        = $35;
    SC_RSHIFT       = $36;
    SC_NP_MULTIPLY  = $37;
{$IFDEF GO32V2}
    SC_ALT          = $38;
{$ENDIF} // DEFINED(GO32V2)
    SC_SPACE        = $39;
    SC_CAPSLOCK     = $3A;
    SC_F1           = $3B;
    SC_F2           = $3C;
    SC_F3           = $3D;
    SC_F4           = $3E;
    SC_F5           = $3F;
    SC_F6           = $40;
    SC_F7           = $41;
    SC_F8           = $42;
    SC_F9           = $43;
    SC_F10          = $44;
    SC_NUMLOCK      = $45;
    SC_SCROLLLOCK   = $46;
    SC_HOME         = $47;
    SC_UP           = $48;
    SC_PAGEUP       = $49;
    SC_NP_MINUS     = $4A;
    SC_LEFT         = $4B;
    SC_RIGHT        = $4D;
    SC_NP_PLUS      = $4E;
    SC_END          = $4F;
    SC_DOWN         = $50;
    SC_PAGEDOWN     = $51;
    SC_INSERT       = $52;
    SC_DELETE       = $53;
    SC_F11          = $57;
    SC_F12          = $58;
{$IFNDEF GO32V2}
{ Fictive scan codes }
    SC_LALT         = $FC;  { Left ALT }
    SC_RALT         = $FD;  { Right ALT }
    SC_LCTRL        = $FE;  { Left CTRL }
    SC_RCTRL        = $FF;  { Right CTRL }
{$ENDIF} // NOT DEFINED(GO32V2)

const
  kF1     = $3b00; kLEFT   = $4b00; kENTER  = $1c0d; kBquote = $2960;
  kF2     = $3c00; kShLEFT = $4b34; kCtENTR = $1c0a; kCtrlE  = $1205;
  kShF2   = $5500; kCtLEFT = $7300; kESC    = $011b; kCtrlG  = $2207;
  kF3     = $3d00; kRIGHT  = $4d00; kTAB    = $0f09; kCtrlQ  = $1011;
  kShF3   = $5600; kShRGHT = $4d36; kShTAB  = $0f00; kCtLbr  = $1a1b;
  kF4     = $3e00; kCtRGHT = $7400; kSPACE  = $3920; kCtRbr  = $1b1d;
  kF5     = $3f00; kUP     = $4800; kBkSPC  = $0e08; kCtrlF3 = $6000;
  kShF5   = $5800; kNPUp   = $4838; kNPastr = $372a; kAltV   = $2f00;
  kF6     = $4000; kDOWN   = $5000; kNPplus = $4e2b; kAstrsk = $092a;
  kShF6   = $5900; kNPDown = $5032; kNPmins = $4a2d; kNPins  = $5230;
  kF7     = $4100; kPgUP   = $4900; kAlt1   = $7800; kNPdel  = $532e;
  kF8     = $4200; kNPPgUP = $4939; kAlt2   = $7900; kCHastr = $092a;
  kShF8   = $5b00; kCtPgUP = $8400; kAlt3   = $7a00; kCtrlP  = $1910;
  kF9     = $4300; kPgDOWN = $5100; kAlt4   = $7b00; kCtBkSp = $0e7f;
  kShF9   = $5c00; kNPPgDN = $5133; kAlt5   = $7c00; kAltF5  = $6c00;
  kF10    = $4400; kCtPgDN = $7600; kAlt6   = $7d00; kAltF6  = $6d00;
  kCtrlB  = $3002; kHOME   = $4700; kAlt7   = $7e00; kAltF8  = $6f00;
  kCtrlC  = $2e03; kNPHOME = $4737; kAlt8   = $7f00; kAltF9  = $7000;
  kCtrlD  = $2004; kCtHOME = $7700; kAlt9   = $8000; kCtrlY  = $1519;
  kCtrlF  = $2106; kEND    = $4f00; kAlt0   = $8100; kAltE   = $1200;
  kCtrlI  = $1709; kNPEND  = $4f31; kAltB   = $3000; kAltZ   = $2c00;
  kCtrlK  = $250b; kCtEND  = $7500; kAltC   = $2e00; kAltI   = $1700;
  kCtrlM  = $320d; kINSERT = $5200; kAltD   = $2000; kAltH   = $2300;
  kCtrlN  = $310e; kDELETE = $5300; kAltF   = $2100; kAltU   = $1600;
  kCtrlO  = $180f; kCtrlL  = $260c; kAltK   = $2500; kAltG   = $2200;
  kCtrlV  = $2f16; kCtrlS  = $1f13; kAltM   = $3200;
  kCtrlX  = $2d18; kCtrlA  = $1e01; kAltN   = $3100;
  kCtrlZ  = $2c1a; kAltF2  = $6900; kAltO   = $1800;
  kAltL   = $2600; kAltA   = $1e00; kAltP   = $1900;
  kCtrlF2 = $5f00; kCHplus = $0d2b; kAltQ   = $1000;
  kAltX   = $2d00; kCHmins = $0c2d; kAltR   = $1300;
  kCtrlT  = $1414; kCHlbrk = $1a5b; kAltS   = $1f00;
  kCtrlR  = $1312; kCHrbrk = $1b5d; kAltT   = $1400;
  kCtrlW  = $1117; kCtrlH  = $2308; kAltW   = $1100;
  kSlash  = $352f; kCtrlF8 = $6500;
  kSlashR = $2b5c; kCtrlF9 = $6600;

{$IFDEF GO32V2}

var
  keyboard_sleep: Boolean; cvar; external;
  CTRL_ALT_DEL_pressed: Boolean; cvar; external;
  _ctrl_pressed: Boolean; cvar; external;
  _2x_ctrl_pressed: Boolean; cvar; external;

{$ENDIF} // DEFINED(GO32V2)

procedure keyboard_init; cdecl; external;
procedure keyboard_done; cdecl; external;
procedure keyboard_reset_buffer; cdecl; external;
{$IFDEF GO32V2}
procedure keyboard_reset_buffer_alt; cdecl; external;
procedure keyboard_toggle_sleep; cdecl; external;
{$ENDIF} // DEFINED(GO32V2)
procedure wait_until_F11_F12_released; cdecl; external;
procedure keyboard_poll_input; cdecl; external;
function  keypressed: Boolean; cdecl; external;
function  GetKey: Word; cdecl; external;
function  scankey(scancode: Byte): Boolean; cdecl; external;
function  CapsLock: Boolean; cdecl; external;
function  NumLock: Boolean; cdecl; external;
{$IFDEF GO32V2}
function  ScrollLock: Boolean; cdecl; external;
{$ENDIF} // DEFINED(GO32V2)
function  shift_pressed: Boolean; cdecl; external;
function  left_shift_pressed: Boolean; cdecl; external;
function  right_shift_pressed: Boolean; cdecl; external;
{$IFDEF GO32V2}
function  both_shifts_pressed: Boolean; cdecl; external;
{$ENDIF} // DEFINED(GO32V2)
function  alt_pressed: Boolean; cdecl; external;
function  ctrl_pressed: Boolean; cdecl; external;
function  ctrl_tab_pressed: Boolean; cdecl; external;
function  LookUpKey(key: Word; var table; size: Byte): Boolean; cdecl; external;
procedure screen_saver; cdecl; external;

implementation

uses
{$IFNDEF GO32V2}
  SDL_Events,
  SDL_Keyboard,
{$ENDIF} // NOT DEFINED(GO32V2)
  pascal;

end.
