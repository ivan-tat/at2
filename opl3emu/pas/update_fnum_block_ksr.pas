// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure update_fnum_block_ksr(p_chan: P_OPL3_CHAN; data: Byte; msb_flag: Boolean);
begin
  If (p_chan^.ch_type = CH_4OP_2) then
    EXIT;

  If msb_flag then
    begin
      // update upper bits
      p_chan^.fnum := (p_chan^.fnum AND $300) OR data;
      p_chan^.ksr := (p_chan^.block SHL 1) OR
                     ((p_chan^.fnum SHR (9 - p_chan^.p_chip^.nts_bit)) AND 1);
    end
  else
    begin
      // update lower bits
      p_chan^.fnum := (p_chan^.fnum AND $0ff) OR
                      ((data AND 3) SHL 8);
      p_chan^.block := (data shr 2) AND 7;
      p_chan^.ksr := (p_chan^.block SHL 1) OR
                     ((p_chan^.fnum SHR (9 - p_chan^.p_chip^.nts_bit)) AND 1);
    end;

  envelope_update_ksl(p_chan^.p_slot[0]);
  envelope_update_ksl(p_chan^.p_slot[1]);
  envelope_update_rate(p_chan^.p_slot[0]);
  envelope_update_rate(p_chan^.p_slot[1]);

  If (p_chan^.ch_type = CH_4OP_1) then
    begin
      p_chan^.p_chan^.fnum := p_chan^.fnum;
      p_chan^.p_chan^.ksr := p_chan^.ksr;

      If msb_flag then
        p_chan^.p_chan^.block := p_chan^.block;

      envelope_update_ksl(p_chan^.p_chan^.p_slot[0]);
      envelope_update_ksl(p_chan^.p_chan^.p_slot[1]);
      envelope_update_rate(p_chan^.p_chan^.p_slot[0]);
      envelope_update_rate(p_chan^.p_chan^.p_slot[1]);
    end;
end;
