// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure ShowCStr(dest: tSCREEN_MEM_PTR; x,y: Byte; str: String; atr1,atr2: Byte);

var
  s: ^Char; // input string
  p: ^Word; // output screen address
  a1,a2,t: Word; // prepared attributes, `t' for swapping
  n: Byte; // number of input characters left to proceed
  c: Char; // character to show
  flag: Byte; // 0=read, 1=stop, 2=show

begin
  n := Length (str);
  if (n <> 0) then begin
    s := @str[1];
    PosChar (x, y); // set `absolute_pos'
    p := Pointer(dest) + absolute_pos; // cast to `Pointer' for correct address
    a1 := atr1 shl 8;
    a2 := atr2 shl 8;
    repeat
      repeat
        c := s^;
        Inc (s);

        if (c = '~') then begin // color switch mark
          t := a2;
          a2 := a1;
          a1 := t;
          Dec (n);
          flag := Ord(n = 0); // check string end
        end else
          flag := 2;

      until (flag <> 0); // color switch mark

      if (flag = 1) then break;

      p^ := Ord(c) + a1;
      Inc (p); // in 2-bytes units

      Dec (n);
    until (n = 0); // check string end
  end;
end;
