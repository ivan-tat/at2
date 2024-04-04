// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure SetSize(columns,lines: Word);
begin
  outportb($3d4,$13);
  outportb($3d5,columns SHR 1);
  MEMW[Seg0040:$4a] := columns;
  MEMW[Seg0040:$84] := lines-1;
  MEMW[Seg0040:$4c] := columns*lines;
end;
