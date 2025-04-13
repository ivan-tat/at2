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

unit Adt2VESA;
{$S-,Q-,R-,V-,B-,X+}
{$PACKRECORDS 1}
{$L go32/adt2vesa.o}
interface

const
  VESA_640x480  = $101;
  VESA_800x600  = $103;
  VESA_1024x768 = $105;

var
  VESA_GraphicsSysInited: Boolean; cvar; external;

  VESA_CallStatus: Byte; cvar; external;
  VESA_Error: Byte; cvar; external;

type
  tModeInfo = Record
                ModeNumber: Word;
                XResolution: Word;
                YResolution: Word;
                BufferAddress: Pointer;
              end;
var
  VESA_NumberOfModes: Byte; cvar; external;
  VESA_FrameBuffer: Pointer; cvar; external;
  VESA_ModeList: array[0..255] of tModeInfo; cvar; external;
  VESA_Version: Word; cvar; external;
  VESA_OEM_String: String; cvar; external;
  VESA_Capabilities: Dword; cvar; external;
  VESA_VideoMemory: Word; cvar; external;
  VESA_OEM_SoftwareRevision: Word; cvar; external;
  VESA_OEM_VendorName: String; cvar; external;
  VESA_OEM_ProductName: String; cvar; external;
  VESA_OEM_ProductRevision: String; cvar; external;
  VESA_Mode: Word; cvar; external;
  VESA_ModeIndex: Byte; cvar; external;
  VESA_XResolution: Word; cvar; external;
  VESA_YResolution: Word; cvar; external;

type
  tPaletteArray = array[0..767] of Byte;

var
  VESA_SegLFB: Word; cvar; external;
  StepWorkPalette: tPaletteArray; cvar; external;
  StepRealPal: array[0..767] of Single; cvar; external;
  StepDelta: array[0..767] of Single; cvar; external;
  FadeSteps: Word; cvar; external;

function  VESA_Init: Longint; cdecl; external;
function  VESA_GetModeInfo(Mode: Word; DOSAddr: Longword): Longint; cdecl; external;
function  VESA_SetMode(Mode: Word): Longint; cdecl; external;
procedure VESA_GetPalette(var Palette); cdecl; external;
procedure VESA_SetPalette(var Palette); cdecl; external;
procedure VESA_InitStepFade(var StartPalette,EndPalette; Speed: Single); cdecl; external;
procedure VESA_StepFade; cdecl; external;
procedure VESA_SwitchBank(Bank: Byte); cdecl; external;

implementation

uses
  GO32;

end.
