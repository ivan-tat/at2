// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure update_instr_data(ins: Byte);

var
  temp: Byte;

begin
  For temp := 1 to 20 do
    If (voice_table[temp] = ins) then
      begin
        reset_chan[temp] := TRUE;
        set_ins_data(ins,temp);
        change_frequency(temp,nFreq(PRED(event_table[temp].note AND $7f))+
                              SHORTINT(ins_parameter(ins,12)));
      end;
end;
