// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function VESA_SetMode(Mode: Word): Integer;

var
  idx: Byte;
  result: Integer;

begin
  If NOT VESA_GraphicsSysInited then
    begin
      VESA_SetMode := -1;
      EXIT;
    end;

  VESA_ModeIndex := VESA_NumberOfModes+1;
  For idx := 0 to VESA_NumberOfModes do
    If (VESA_ModeList[idx].ModeNumber = Mode) then
      begin
        Write(VESA_ModeList[idx].ModeNumber,',');
        VESA_ModeIndex := idx;
        BREAK;
      end;

  If (VESA_ModeIndex = VESA_NumberOfModes+1) then
    begin
      VESA_SetMode := -1;
      EXIT;
    end;

  If (Mode <> 19) then Mode := Mode OR $4000;
  asm
      mov       ax,4f02h
      mov       bx,Mode
      int       10h
      mov       result,0
      cmp       ax,4fh
      je        @@1
      mov       result,-1
@@1:
  end;

  VESA_SetMode := result;
  If (result <> 0) then EXIT;

  VESA_Mode := Mode;
  VESA_XResolution := VESA_ModeList[VESA_ModeIndex].XResolution;
  VESA_YResolution := VESA_ModeList[VESA_ModeIndex].YResolution;
  VESA_Framebuffer := VESA_ModeList[VESA_ModeIndex].BufferAddress;
end;
