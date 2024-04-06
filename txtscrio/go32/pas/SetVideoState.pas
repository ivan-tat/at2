// This file is part of Adlib Tracker II (AT2).
//
// Saved from `adt2play.pas'.
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

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
