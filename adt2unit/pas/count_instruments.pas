// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure count_instruments(var instruments: Byte);
begin
  _dbg_enter ({$I %FILE%}, 'count_instruments');

  instruments := 255;
  While (instruments > 0) and
        Empty(songdata.instr_data[instruments],INSTRUMENT_SIZE) do
    Dec(instruments);

  _dbg_leave; //EXIT //count_instruments
end;
