// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function CStrLen(str: String): Byte;

var
  s: ^Char; // input string
  n: Byte; // number of input characters left to proceed
  len: Byte; // actual number of printable characters

begin
  len := 0;

  n := Length (str);
  s := @str[1];
  while (n <> 0) do begin
    if (s^ <> '~') then // color switch mark
      Inc (len);
    Inc (s);
    Dec (n);
  end;

  CStrLen := len;
end;
