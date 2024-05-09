// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function byte2hex(value: Byte): String;

const
  data: array[0..15] of char = '0123456789ABCDEF';

begin
  byte2hex := data[value AND $0f0 SHR 4]+
              data[value AND $0f];
end;
