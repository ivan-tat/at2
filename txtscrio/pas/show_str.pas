// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure show_str(xpos,ypos: Byte; str: String; color: Byte);

var
  s: ^Char; // input string
  p: Pointer; // output screen address
  a: Word; // prepared attribute
  n: Byte; // number of input characters left to proceed
  i: Byte; // index for a new character to show (1-based)
  x12,x21,x22,y11,y21,x,y: Byte;
  c: Char; // character to show

begin
  n := Length (str);
  if (n <> 0) then begin
    s := @str[1];

    x12 := area_x1 + 2; // shadow: bottom line X1
    x21 := area_x2 + 1; // shadow: vertical rectangle X1
    x22 := area_x2 + 2; // shadow: bottom line and vertical rectangle X2
    y11 := area_y1 + 1; // shadow: vertical rectangle Y1
    y21 := area_y2 + 1; // shadow: bottom line Y and vertical rectangle Y2

    i := 1;
    a := color shl 8;
    repeat
      c := s^;
      Inc (s);

      p := Pointer(screen_ptr) + (((ypos-1)*MaxCol + xpos+i-2) shl 1);

      x := xpos+i-1;
      y := ypos;
      if ((x >= x12) and (x <= x22) and (y = y21)
      or  (x >= x21) and (x <= x22) and (y >= y11) and (y <= y21)) then begin
        // in the shadow
        Char(p^) := c;
      end else
      if ((x < area_x1) or (x > area_x2)
      or  (y < area_y1) or (y > area_y2)) then begin
        // outside the area
        Word(p^) := Ord(c) + a;
      end;

      Inc (i);
    until (i > n); // check string end
  end;
end;
