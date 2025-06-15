// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Local to `valid_drive' function

function phantom_drive(drive: Char): Boolean;

var
  regs: tRealRegs;

begin
  _dbg_enter ({$I %FILE%}, 'valid_drive.phantom_drive');

  regs.ax := $440e;
  regs.bl := BYTE(UpCase(drive))-$40;
  RealIntr($21,regs);

  If Odd(regs.flags) then phantom_drive := FALSE
  else If (regs.al = 0) then phantom_drive := FALSE
       else phantom_drive := (regs.al <> BYTE(UpCase(drive))-$40);

  _dbg_leave; //EXIT //valid_drive.phantom_drive
end;
