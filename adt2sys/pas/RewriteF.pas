// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure RewriteF(var f: File);

var
  fattr: Word;

begin
  _dbg_enter ({$I %FILE%}, 'RewriteF');

  GetFAttr(f,fattr);
  If (fattr AND ReadOnly = ReadOnly) then
    SetFAttr(f,fattr AND NOT ReadOnly);
  {$i-}
  Rewrite(f,1);
  {$i+}

  _dbg_leave; //EXIT //RewriteF
end;
