// This file is part of Adlib Tracker II (AT2).
//
// Saved from `adt2play/txtscrio.pas'.
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

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
