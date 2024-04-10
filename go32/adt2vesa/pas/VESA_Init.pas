// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure VESA_Init;

var
  idx,idx2: Byte;
  regs: tRealRegs;
  dos_sel,dos_seg: Word;
  dos_mem_adr: Dword;

function GetVESAInfoStr(dpmiStrPtr: Dword): String;
begin
  GetVESAInfoStr := StrPas(PCHAR(POINTER(Ofs(VESA_Info)+WORD(dpmiStrPtr))));
end;

begin
  If VESA_GraphicsSysInited then EXIT;
  With VESA_ModeList[0] do
    begin
      ModeNumber := $13;
      XResolution := 320;
      YResolution := 200;
      BufferAddress := POINTER($A0000);
    end;

  dos_mem_adr := global_dos_alloc(SizeOf(tVESA_Info));
  dos_sel := WORD(dos_mem_adr);
  dos_seg := WORD(dos_mem_adr SHR 16);

  FillChar(VESA_Info,SizeOf(tVESA_Info),0);
  VESA_Info.Signature := 'VBE2';
  dosmemput(dos_seg,0,VESA_Info,4);

  regs.ax := $4f00;
  regs.ds := dos_seg;
  regs.es := dos_seg;
  regs.di := 0;
  RealIntr($10,regs);

  dosmemget(dos_seg,0,VESA_Info,SizeOf(tVESA_Info));
  global_dos_free(dos_sel);

  If (VESA_Info.Signature <> 'VESA') then
    EXIT; // ERROR: VESA BIOS extensions not found!
  VESA_Version := VESA_Info.Version;

  If (HI(VESA_Version) < 2) then
    EXIT; // ERROR: VESA 2.0 required!

  VESA_OEM_String := GetVESAInfoStr(VESA_Info.OEM_StringPtr);
  VESA_Capabilities := VESA_Info.Capabilities;
  VESA_VideoMemory := VESA_Info.VideoMemory SHL 6;
  VESA_OEM_SoftwareRevision := VESA_Info.OEM_SoftwareRevision;
  VESA_OEM_VendorName := GetVESAInfoStr(VESA_Info.OEM_VendorNamePtr);
  VESA_OEM_ProductName := GetVESAInfoStr(VESA_Info.OEM_ProductNamePtr);
  VESA_OEM_ProductRevision := GetVESAInfoStr(VESA_Info.OEM_ProductRevisionPtr);

  dpmi_dosmemget(WORD(VESA_Info.ModeListPtr SHR 16),
                 WORD(VESA_Info.ModeListPtr),
                 ModeList,
                 SizeOf(tModeList));

  dos_mem_adr := global_dos_alloc(SizeOf(tModeInfoBlock));
  dos_sel := WORD(dos_mem_adr);
  dos_seg := WORD(dos_mem_adr SHR 16);

  idx  := 0;
  idx2 := 1;

  Repeat
    regs.ax := $4f01;
    regs.cx := ModeList[idx];
    regs.ds := dos_seg;
    regs.es := dos_seg;
    regs.di := 0;
    RealIntr($10,regs);
    dosmemget(dos_seg,0,ModeInfoBlock,SizeOf(tModeInfoBlock));
    Inc(idx);
  until ((ModeInfoBlock.ModeAttributes AND $0091 = $0091) and
         (ModeInfoBlock.NumberOfPlanes = 1) and
         (ModeInfoBlock.BitsPerPixel = 8)) or (ModeList[idx-1] = $FFFF);

  If (ModeList[idx-1] <> $FFFF) then
    begin
      Inc(idx2);
      With VESA_ModeList[1] do
        begin
          ModeNumber := ModeList[idx-1];
          XResolution := ModeInfoBlock.XResolution;
          YResolution := ModeInfoBlock.YResolution;
          BufferAddress := ModeInfoBlock.PhysBasePtr;
        end;

      While (idx <= 127) and (ModeList[idx] <> $FFFF) do
        begin
          regs.ax := $4f01;
          regs.cx := ModeList[idx];
          regs.ds := dos_seg;
          regs.es := dos_seg;
          regs.di := 0;
          RealIntr($10,regs);
          dosmemget(dos_seg,0,ModeInfoBlock,SizeOf(tModeInfoBlock));
          If (ModeInfoBlock.ModeAttributes AND $0091 = $0091) and
             (ModeInfoBlock.NumberOfPlanes = 1) and
             (ModeInfoBlock.BitsPerPixel = 8) then
            begin
              With VESA_ModeList[idx2] do
                begin
                  ModeNumber := ModeList[idx];
                  XResolution := ModeInfoBlock.XResolution;
                  YResolution := ModeInfoBlock.YResolution;
                end;
              Inc(idx2);
            end;
          Inc(idx);
        end;
    end;

  global_dos_free(dos_sel);
  VESA_NumberOfModes := idx2-1;

  If (VESA_NumberOfModes >= 1) then
    begin
      VESA_ModeList[1].BufferAddress :=
        POINTER(DWORD(Get_Linear_Addr(DWORD(VESA_ModeList[1].BufferAddress),4096*1024)));
      If (VESA_ModeList[1].BufferAddress = NIL) then
        EXIT; // ERROR: Cannot remap LFB to linear address space!
      For idx := 2 to VESA_NumberOfModes do
        VESA_ModeList[idx].BufferAddress := VESA_ModeList[1].BufferAddress;
    end;

   VESA_Mode := 0;
  VESA_GraphicsSysInited := TRUE;
end;
