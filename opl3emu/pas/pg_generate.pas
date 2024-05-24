// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure pg_generate(p_slot: P_OPL3_SLOT);

var
  fnum: Word;
  fnum_hi: Byte;

begin
  fnum := p_slot^.p_chan^.fnum;
  If (p_slot^.reg_vib <> 0) then
    begin
      // apply LFO vibrato
      fnum_hi := fnum SHR (7 + VIB_SHIFT[(p_slot^.p_chip^.timer SHR 10) AND 7] +
                               (1 - p_slot^.p_chip^.dvb_bit));
      Inc(fnum,fnum_hi * VIB_S_SHIFT[(p_slot^.p_chip^.timer SHR 10) AND 7]);
    end;

  Inc(p_slot^.pg_phase,(((fnum SHL p_slot^.p_chan^.block) SHR 1) *
                        MULT_VAL[p_slot^.reg_mult]) SHR 1);
end;
