// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure slot_generate(p_slot: P_OPL3_SLOT; phase: Word);
begin
  If (phase = WORD_NULL) then
    phase := WORD(p_slot^.pg_phase SHR 9) + p_slot^.p_mod^;
  p_slot^.output := envelope_calc_sin(p_slot^.reg_wf,phase,p_slot^.eg_out);
end;
