// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure new_exit_proc;

var
  temp: Byte;

begin
  asm mov ax,03h; xor bh,bh; int 10h end;
  ExitProc := @old_exit_proc;

  If (ExitCode <> 0) then
    begin
      WriteLn('‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹');
      WriteLn('€ ABNORMAL PROGRAM TERMiNATiON €');
      WriteLn('ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ');
      WriteLn('PROGRAM VERSION: ',VERSION_STR);
      WriteLn('ERROR #'+Num2str(ExitCode,10)+' at '+ExpStrL(Num2str(LONGINT(ErrorAddr),16),8,'0'));
      _dbg_dump_callstack;
      WriteLn;
      WriteLn('Please send this information with brief description what you were doing');
      WriteLn('when you encountered this error to following email address:');
      WriteLn;
      WriteLn('subz3ro.altair@gmail.com');
      WriteLn;
      WriteLn('Thanks and sorry for your inconvenience! :-)');
      ErrorAddr := NIL;
      HALT(ExitCode);
    end
  else HALT(0);
end;
