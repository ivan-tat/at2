// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure SetRGBitem(color: Byte; red,green,blue: Byte);
begin
  outportb($3c8,color);
  outportb($3c9,red);
  outportb($3c9,green);
  outportb($3c9,blue);
end;
