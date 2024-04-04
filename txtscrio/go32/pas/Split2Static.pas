// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure Split2Static;
begin
  inportb($3da);
  outportb($3c0,$10 OR $20);
  outportb($3c0,inportb($3c1) OR $20);
end;
