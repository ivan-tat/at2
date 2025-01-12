// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure ResetF(var f: File);

var
  fattr: Word;

begin
  _dbg_enter ({$I %FILE%}, 'ResetF');

  GetFAttr(f,fattr);
  If (fattr AND ReadOnly = ReadOnly) then
    FileMode := 0;
  {$i-}
  Reset(f,1);
  {$i+}

  _dbg_leave; //EXIT //ResetF
end;
