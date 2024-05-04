// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function Num2str(num: Longint; base: Byte): String;

const
  hexa: array[0..PRED(16)] of Char = '0123456789ABCDEF';

var
  result: String;

begin
  result := '';
  If (base >= 2) and (base <= 16) then
    While (num > 0) do
      begin
        result := hexa[num MOD base]+result;
        num := num DIV base;
      end;
  If (result = '') then Num2str := '0'
  else Num2str := result;
end;
