// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Saved from "dialogio.pas"

function _partial(max,val: Word; base: Byte): Word;

var
  temp1,temp2: Real;
  temp3: Word;

begin
  temp1 := max/base;
  temp2 := (max/base)/2;
  temp3 := 0;
  While (temp2 < val) do
    begin
      temp2 := temp2+temp1;
      Inc(temp3);
    end;
  _partial := temp3;
end;
