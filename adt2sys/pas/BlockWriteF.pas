// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure BlockWriteF(var f: File; var data; size: Longint; var bytes_written: Longint);
begin
  _dbg_enter ({$I %FILE%}, 'BlockWriteF');

  {$i-}
  BlockWrite(f,data,size,bytes_written);
  {$i+}
  If (IOresult <> 0) then
    bytes_written := 0;

  _dbg_leave; //EXIT //BlockWriteF
end;
