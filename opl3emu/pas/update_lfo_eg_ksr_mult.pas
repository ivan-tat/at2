// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure update_lfo_eg_ksr_mult(p_slot: P_OPL3_SLOT; data: Byte);
begin
  // assign LFO tremolo
  If ((data SHR 7) AND 1 <> 0) then
    p_slot^.p_trem := @p_slot^.p_chip^.trem_val
  else p_slot^.p_trem := @p_slot^.p_chip^.out_null;

  p_slot^.reg_vib := (data SHR 6) AND 1;
  p_slot^.reg_type := (data SHR 5) AND 1;
  p_slot^.reg_ksr := (data SHR 4) AND 1;
  p_slot^.reg_mult := data AND $0f;
  envelope_update_rate(p_slot);
end;
