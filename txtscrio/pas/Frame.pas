// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure Frame(dest: tSCREEN_MEM_PTR; x1,y1,x2,y2,atr1: Byte;
                title: String; atr2: Byte; border: String);
var
  s: ^Char; // input string
  p: Pointer; // output screen address
  a: Word; // prepared attribute
  xexp1,xexp2,xexp3,yexp1,yexp2: ShortInt;
  x,y,n: Byte;
  wide_range_type: Boolean;
  shadow_enabled: Boolean;

begin
  wide_range_type := fr_setting.wide_range_type;
  shadow_enabled := fr_setting.shadow_enabled;
  if (fr_setting.update_area) then begin
    area_x1 := x1;
    area_y1 := y1;
    area_x2 := x2;
    area_y2 := y2;
  end;

  if (wide_range_type) then begin
    xexp1 := 4;
    xexp2 := -1;
    xexp3 := 7;
    yexp1 := 1;
    yexp2 := 2;
    DupChar (x1-3, y1-1, ' ', atr1, x2-x1+7, dest);
    DupChar (x1-3, y2+1, ' ', atr1, x2-x1+7, dest);
    y := y1;
    repeat
      DupChar (x1-3, y, ' ', atr1, 3, dest);
      DupChar (x2+1, y, ' ', atr1, 3, dest);
      Inc (y);
    until (y > y2); // signed comparison (!?)
  end else begin
    xexp1 := 1;
    xexp2 := 2;
    xexp3 := 1;
    yexp1 := 0;
    yexp2 := 1;
  end;

  n := x2-x1-1;
  DupChar (x1,   y1, border[1], atr1, 1, dest);
  DupChar (x1+1, y1, border[2], atr1, n, dest);
  DupChar (x2,   y1, border[3], atr1, 1, dest);

  y := y1;
  repeat
    Inc (y);
    DupChar (x1,   y, border[4], atr1, 1, dest);
    DupChar (x1+1, y, ' ',       atr1, n, dest);
    DupChar (x2,   y, border[5], atr1, 1, dest);
  until (y >= y2); // signed comparison (!?)

  DupChar (x1,   y2, border[6], atr1, 1, dest);
  DupChar (x1+1, y2, border[7], atr1, n, dest);
  DupChar (x2,   y2, border[8], atr1, 1, dest);

  n := Length (title);
  if (n <> 0) then begin
    x := x2-x1-n;
    x := Word(x+1) shr 1;
    PosChar (x1+x, y1); // set `absolute_pos'
    p := Pointer(dest) + absolute_pos;
    s := @title[1];
    a := atr2 shl 8;
    repeat
      Word(p^) := Ord(s^) + a;
      Inc (s);
      Inc (p, 2);
      Dec (n);
    until (n = 0);
  end;

  if (shadow_enabled) then begin
    y := y1-yexp1;
    repeat
      Inc (y);
      PosChar (x2+xexp1, y); // set `absolute_pos'
      p := Pointer(dest) + absolute_pos;
      Byte((p+1)^) := $07;
      Byte((p+3)^) := $07;
      Inc (p, 5);
      if (MaxCol > 180) then begin
        Byte((p+1)^) := $07;
        Inc (p, 2);
      end;
    until (y > y2);

    PosChar (x1+xexp2, y2+yexp2); // set `absolute_pos'
    p := Pointer(dest) + absolute_pos + 1;
    n := x2-x1+xexp3;
    if (MaxLn >= 60) then Dec (n);
    repeat
      Byte(p^) := $07;
      Inc (p, 2);
      Dec (n);
    until (n = 0);
  end;
end;
