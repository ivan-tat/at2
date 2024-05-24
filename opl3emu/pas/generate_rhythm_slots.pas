// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure generate_rhythm_slots(p_chip: P_OPL3_CHIP);

var
  p_slot: P_OPL3_SLOT;
  phase,phase_lo,phase_hi,phase_bit: Word;

procedure calc_phase_slot7_slot8;
begin
  phase_lo := (p_chip^.chan[7].p_slot[0]^.pg_phase SHR 9) AND $3ff;
  phase_hi := (p_chip^.chan[8].p_slot[1]^.pg_phase SHR 9) AND $3ff;

  If ((phase_lo AND 8) OR
      (((phase_lo SHR 5) XOR phase_lo) AND 4) OR
      (((phase_hi SHR 2) XOR phase_hi) AND 8) <> 0) then
    phase_bit := 1
  else
    phase_bit := 0;
end;

begin
  // BD
  p_slot := p_chip^.chan[6].p_slot[0];
  slot_generate(p_slot,WORD(p_slot^.pg_phase SHR 9) + (p_slot^.p_mod^));
  p_slot := p_chip^.chan[6].p_slot[1];
  slot_generate(p_slot,WORD(p_slot^.pg_phase SHR 9) + (p_slot^.p_mod^));

  // HH
  p_slot := p_chip^.chan[7].p_slot[0];
  calc_phase_slot7_slot8;
  phase := (phase_bit SHL 9) OR
           WORD($34 SHL ((phase_bit XOR (p_chip^.noise AND 1) SHL 1)));
  slot_generate(p_slot,phase);

  // TT
  p_slot := p_chip^.chan[8].p_slot[0];
  slot_generate(p_slot,WORD(p_slot^.pg_phase SHR 9));

  // SD
  p_slot := p_chip^.chan[7].p_slot[1];
  calc_phase_slot7_slot8;
  phase := ($100 SHL ((phase_lo SHR 8) AND 1)) XOR
           WORD((p_chip^.noise AND 1) SHL 8);
  slot_generate(p_slot,phase);

  // TC
  p_slot := p_chip^.chan[8].p_slot[1];
  phase := $100 OR (phase_bit SHL 9);
  slot_generate(p_slot,phase);
end;
