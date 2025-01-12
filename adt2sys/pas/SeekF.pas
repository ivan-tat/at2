// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure SeekF(var f: File; fpos: Longint);
begin
  _dbg_enter ({$I %FILE%}, 'SeekF');

  {$i-}
  Seek(f,fpos);
  {$i+}

  _dbg_leave; //EXIT //SeekF
end;
