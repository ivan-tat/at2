// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function calc_max_speedup(tempo: Byte): Word;

var
  temp: Longint;
  result: Word;

begin
  _dbg_enter ({$I %FILE%}, 'calc_max_speedup');

  result := MAX_IRQ_FREQ DIV tempo;
  Repeat
    If (tempo = 18) and timer_fix then temp := TRUNC((tempo+0.2)*20)
    else temp := 250;
    While (temp MOD (tempo*result) <> 0) do Inc(temp);
    If (temp <= MAX_IRQ_FREQ) then Inc(result);
  until NOT (temp <= MAX_IRQ_FREQ);
  calc_max_speedup := PRED(result);

  _dbg_leave; //EXIT //calc_max_speedup
end;
