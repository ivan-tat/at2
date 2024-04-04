// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure GetRGBitem(color: Byte; var red,green,blue: Byte);
begin
  outportb($3c7,color);
  red   := inportb($3c9);
  green := inportb($3c9);
  blue  := inportb($3c9);
end;
