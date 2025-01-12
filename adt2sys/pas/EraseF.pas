// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure EraseF(var f: File);
begin
  _dbg_enter ({$I %FILE%}, 'EraseF');

  {$i-}
  Erase(f);
  {$i+}
  If (IOresult <> 0) then ;

  _dbg_leave; //EXIT //EraseF
end;
