// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function byte2dec(value: Byte): String;

const
  data: array[0..9] of char = '0123456789';

begin
  If (value < 100) then
    byte2dec := data[value DIV 10]+
                data[value MOD 10]
  else
    byte2dec := data[value DIV 100]+
                data[value MOD 100 DIV 10]+
                data[value MOD 100 MOD 10];
end;
