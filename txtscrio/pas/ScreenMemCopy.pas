// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure ScreenMemCopy(source,dest: tSCREEN_MEM_PTR);
begin
  cursor_backup := GetCursor;
  Move (source^, dest^, SCREEN_MEM_SIZE);
end;
