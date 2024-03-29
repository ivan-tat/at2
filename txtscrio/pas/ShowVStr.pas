// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure ShowVStr(dest: tSCREEN_MEM_PTR; x,y: Byte; str: String; attr: Byte);

var
  s: ^Char; // input string
  p: ^Word; // output screen address
  a: Word; // prepared attribute
  n: Byte; // number of input characters left to proceed

begin
  PosChar (x, y); // set `absolute_pos'

  n := Length (str);
  if (n <> 0) then begin
    s := @str[1];
    p := Pointer(dest) + absolute_pos; // cast to `Pointer' for correct address
    a := attr shl 8;
    repeat
      p^ := Ord(s^) + a;
      Inc (s);
      Inc (p, MaxCol); // in 2-bytes units
      Dec (n);
    until (n = 0);
  end;
end;
