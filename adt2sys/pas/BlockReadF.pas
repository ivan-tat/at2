// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure BlockReadF(var f: File; var data; size: Longint; var bytes_read: Longint);
begin
  _dbg_enter ({$I %FILE%}, 'BlockReadF');

  {$i-}
  BlockRead(f,data,size,bytes_read);
  {$i+}
  If (IOresult <> 0) then
    bytes_read := 0;

  _dbg_leave; //EXIT //BlockReadF
end;
