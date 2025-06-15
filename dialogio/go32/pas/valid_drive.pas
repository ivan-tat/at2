// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function valid_drive(drive: Char): Boolean;

{$I phantom_drive.pas}

var
  regs: tRealRegs;
  dos_sel,dos_seg: Word;
  dos_mem_adr: Dword;
  dos_data: array[0..PRED(40)] of Byte;

begin
  _dbg_enter ({$I %FILE%}, 'valid_drive');

  dos_mem_adr := global_dos_alloc(40);
  dos_sel := WORD(dos_mem_adr);
  dos_seg := WORD(dos_mem_adr SHR 16);

  dos_data[0] := BYTE(UpCase(drive));
  dos_data[1] := BYTE(':');
  dos_data[2] := 0;
  dosmemput(dos_seg,0,dos_data,40);

  regs.ax := $2906;
  regs.si := 0;
  regs.di := 3;
  regs.ds := dos_seg;
  regs.es := dos_seg;
  RealIntr($21,regs);

  global_dos_free(dos_sel);
  valid_drive := (regs.al <> BYTE_NULL) and NOT phantom_drive(drive);

  _dbg_leave; //EXIT //valid_drive
end;
