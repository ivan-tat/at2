// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function envelope_calc_rate(p_slot: P_OPL3_SLOT; reg_rate: Byte): Byte;

var
  rate: Byte;

begin
  If (reg_rate = 0) then
    begin
      envelope_calc_rate := 0;
      EXIT; //envelope_calc_rate
    end;

  rate := (reg_rate SHL 2);
  If (p_slot^.reg_ksr <> 0) then
    Inc(rate,p_slot^.p_chan^.ksr)
  else
    Inc(rate,(p_slot^.p_chan^.ksr SHR 2));
  envelope_calc_rate := limit_value(rate,0,60);

  //EXIT //envelope_calc_rate
end;
